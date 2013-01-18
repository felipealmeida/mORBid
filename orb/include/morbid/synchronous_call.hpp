/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_SYNCHRONOUS_CALL_HPP
#define MORBID_SYNCHRONOUS_CALL_HPP

#include <morbid/detail/max_args.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/structured_ior.hpp>
#include <morbid/struct_fusion.hpp>
#include <morbid/arguments_traits.hpp>
#include <morbid/giop/grammars/message_1_0.hpp>
#include <morbid/giop/grammars/request_1_0.hpp>
#include <morbid/giop/grammars/reply_1_0.hpp>
#include <morbid/giop/grammars/system_exception_reply_body.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
#include <morbid/giop/grammars/arguments.hpp>
#include <morbid/iiop/all.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/remove_if.hpp>
#include <boost/algorithm/hex.hpp>

#include <string>
#include <typeinfo>
#include <iostream>
#include <iterator>

namespace std {

inline ostream& operator<<(ostream& os, vector<char>::iterator it)
{
  return os << "[ char iterator ]";
}

}

namespace morbid { namespace synchronous_call {

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

struct remove_value_type;

template <typename F>
struct remove_value_type_result_of;

template <typename A>
struct remove_value_type_result_of<remove_value_type(A)>
{
  typedef typename boost::remove_const
    <typename boost::remove_reference<A>::type>::type::type type;
};

struct remove_value_type
{
  template <typename F>
  struct result : remove_value_type_result_of<F> {};

  template <typename A>
  typename result<remove_value_type(A)>::type
  operator()(A a) const
  {
    return a.value;
  }
};

template <typename O, typename T>
struct use_manage_types_result_of
{
  typedef T type;
  type const& operator()(type const& o) const { return o; }
};

template <typename T>
struct use_manage_types_result_of<morbid::String, T>
{
  typedef type_tag::value_type_tag<std::string, typename T::tag> type;
  type operator()(T const& t) const { return type(t.value); }
};

struct use_manage_types
{
  template <typename F>
  struct result;

  template <typename This, typename A>
  struct result<This(A)> : use_manage_types_result_of
  <typename boost::remove_const<typename boost::remove_reference<A>::type>::type::original_type
   , typename boost::remove_const<typename boost::remove_reference<A>::type>::type> {};

  template <typename A>
  typename result<use_manage_types(A)>::type
  operator()(A const& a) const
  {
    typedef result<use_manage_types(A)> type;
    return type()(a);
  }
};

template <std::size_t S, typename T>
struct in_args_one_elem_converter
{
  typedef T type;
  
  T operator()(T const& s) const
  {
    return s;
  }
};

template <typename T>
struct in_args_one_elem_converter<1u, T>
{
  typedef typename boost::remove_reference<typename fusion::result_of::at_c<T, 0u>::type>::type type;
  
  type operator()(T const& s) const
  { return fusion::at_c<0u>(s); }
};

template <typename Src, typename DstSeq>
void aux_copy(Src const& src, DstSeq const& dst_seq
              , mpl::identity<fusion::non_fusion_tag>)
{
  std::cout << "Copying from out arg (with type " << typeid(src).name() << ") " << src << std::endl;
  fusion::at_c<0u>(dst_seq).value = src;
}

template <typename SrcSeq, typename DstSeq, typename Tag>
void aux_copy(SrcSeq const& src_seq, DstSeq const& dst_seq
              , mpl::identity<Tag>)
{
  fusion::copy(src_seq, dst_seq);
}

template <typename R, typename OutResult, typename OutSeq, typename Tag>
typename return_traits<R>::type return_value(mpl::identity<R>, OutResult const& seq, OutSeq const& out
                                             , mpl::identity<Tag> tag
                                             , typename boost::disable_if<boost::is_same<void, R> >::type* = 0)
{
  aux_copy(fusion::pop_front(seq), out, tag);
  return fusion::front(seq);
}

template <typename R, typename OutResult, typename OutSeq>
typename return_traits<R>::type return_value(mpl::identity<R>, OutResult const& r, OutSeq const&
                                             , mpl::identity<fusion::non_fusion_tag>
                                             , typename boost::disable_if<boost::is_same<void, R> >::type* = 0)
{
  return r;
}

template <typename OutResult, typename OutSeq, typename Tag>
void return_value(mpl::identity<void>, OutResult const& seq, OutSeq const& out
                  , mpl::identity<Tag> tag)
{
  aux_copy(seq, out, tag);
}

template <typename R, typename ArgsSeq>
typename return_traits<R>::type call
  (const char* repoid, const char* method, structured_ior const& ior, ArgsSeq args)
{
  namespace mpl = boost::mpl;
  if(ior.structured_profiles.empty())
    throw MARSHALL();

  iiop::profile_body const* profile_body = 0;
  for(std::vector<structured_ior::variant_type>::const_iterator
        first = ior.structured_profiles.begin()
        , last = ior.structured_profiles.end()
        ;first != last; ++first)
  {
    if((profile_body = boost::get<iiop::profile_body const>(&*first)))
      break;
  }

  if(!profile_body)
    throw MARSHALL();

  std::string host = profile_body->host;
  std::vector<char> object_key = profile_body->object_key;
  unsigned short port = profile_body->port;

  std::cout << "Making synchronous call to " << host << ":" << port
            << " to object with object_key " << boost::make_iterator_range(object_key.begin(), object_key.end())
            << " with repoid: " << repoid << " to method " << method
            << " and args [" << typeid(args).name()
            << ']' << std::endl;

  namespace fusion = boost::fusion;
  namespace karma = boost::spirit::karma;

  typedef typename mpl::lambda<type_tag::is_not_out_type_tag<mpl::_1> >::type is_not_out_lambda;
  typedef typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type is_not_in_lambda;
  typedef typename mpl::lambda<type_tag::is_out_type_tag<mpl::_1> >::type is_out_lambda;
  typedef typename mpl::lambda<type_tag::is_in_type_tag<mpl::_1> >::type is_in_lambda;

  typedef typename mpl::remove_if
    <ArgsSeq const, is_out_lambda>::type in_args_type_seq_type;

  typedef typename fusion::result_of::as_vector
    <typename fusion::result_of::transform
     <typename fusion::result_of::transform
      <typename fusion::result_of::filter_if<ArgsSeq const, is_not_out_lambda>::type
       , use_manage_types>::type
      , remove_value_type>::type>::type in_args_type_;
  in_args_type_ in_args_ = fusion::as_vector
    (fusion::transform
     (fusion::transform
      (fusion::filter_if<is_not_out_lambda>(args)
       , use_manage_types())
      , remove_value_type()
     ))
    ;

  typedef typename in_args_one_elem_converter<mpl::size<in_args_type_>::value, in_args_type_>::type in_args_type;
  in_args_type in_args = in_args_one_elem_converter<mpl::size<in_args_type_>::value, in_args_type_>()(in_args_);

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;

  typedef std::vector<fusion::vector2<unsigned int, std::vector<char> > > service_context_list;
  typedef fusion::vector7<service_context_list
                          , unsigned int, bool, std::vector<char>, std::string, std::vector<char>
                          , in_args_type const&>
    request_attribute_type;
  typedef fusion::vector1<fusion::vector1<request_attribute_type> >
    message_attribute_type;

  typedef giop::grammars::arguments<iiop::generator_domain
                                    , output_iterator_type, in_args_type_seq_type, in_args_type>
    arguments_grammar;
  typedef giop::grammars::request_1_0<iiop::generator_domain
                                      , output_iterator_type, request_attribute_type>
    request_header_grammar;
  typedef giop::grammars::message_1_0<iiop::generator_domain
                                      , output_iterator_type, message_attribute_type
                                      , 0u /* request */>
    message_header_grammar;

  morbid::arguments_traits arguments_traits;
  arguments_grammar arguments_grammar_(arguments_traits);
  request_header_grammar request_header_grammar_(arguments_grammar_);
  message_header_grammar message_header_grammar_(request_header_grammar_);
  message_attribute_type attribute (fusion::make_vector
                                    (request_attribute_type
                                     (service_context_list(), 1u, true, object_key
                                      , method, std::vector<char>(), in_args)));
  
  std::vector<char> buffer;
  output_iterator_type iterator(buffer);
  if(karma::generate(iterator, giop::compile<iiop::generator_domain>
                     (message_header_grammar_(giop::native_endian))
                     , attribute))
  {
    std::cout << "Generated" << std::endl;
    boost::algorithm::hex(buffer.begin(), buffer.end()
                          , std::ostream_iterator<char>(std::cout));
    std::cout << std::endl;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query
      (boost::asio::ip::tcp::endpoint::protocol_type::v4(), host, "");
    boost::asio::ip::tcp::endpoint remote_endpoint = *resolver.resolve(query);
    remote_endpoint.port(port);
    boost::asio::ip::tcp::socket socket(io_service, boost::asio::ip::tcp::endpoint());
    socket.connect(remote_endpoint);
    boost::asio::write(socket, boost::asio::buffer(buffer)
                       , boost::asio::transfer_all());

    std::vector<char> reply_buffer(1024u);
    std::size_t offset = 0;
    std::vector<char>::iterator first;
    do
    {
      if(reply_buffer.size() - offset < 128u)
        reply_buffer.resize(reply_buffer.size() + 1024u);
      boost::system::error_code ec;
      std::size_t bytes_read
        = socket.read_some(boost::asio::buffer(&reply_buffer[0], reply_buffer.size() - offset), ec);
      std::cout << "Read " << bytes_read << " bytes" << std::endl;
      if(ec)
        throw ec;
      else
      {
        offset += bytes_read;
        typedef std::vector<char>::iterator iterator_type;
        iterator_type first = reply_buffer.begin()
          ,  last = boost::next(reply_buffer.begin(), offset);

        std::cout << "Should try to parse" << std::endl;

        typedef typename mpl::remove_if
          <ArgsSeq const, is_in_lambda>::type out_args_type_seq_type;

        std::cout << "out_args_type_seq_type " << typeid(out_args_type_seq_type).name() << std::endl;

        typedef typename mpl::transform
           <typename fusion::result_of::transform
           <typename fusion::result_of::filter_if<ArgsSeq const, is_not_in_lambda>::type
            , remove_value_type>::type
            , boost::remove_reference<mpl::_1>
            , mpl::back_inserter<mpl::vector0<> >
            >::type
           out_args_type_;

        
        std::cout << "out_args_type_ " << typeid(out_args_type_).name() << std::endl;

        typedef typename fusion::result_of::as_vector
          <typename mpl::eval_if
          <boost::is_same<R, void>
           , mpl::identity<out_args_type_>
           , fusion::result_of::push_front
             <out_args_type_, typename return_traits<R>::type>
           >::type>::type arguments_attribute_type_;
        typedef typename fusion::result_of::as_vector
          <typename mpl::eval_if
          <boost::is_same<R, void>
           , mpl::identity<out_args_type_seq_type>
           , fusion::result_of::push_front
             <out_args_type_seq_type, type_tag::value_type_tag<R, type_tag::out_tag> >
          >::type>::type arguments_attribute_seq_type;

        typedef typename in_args_one_elem_converter
          <mpl::size<arguments_attribute_type_>::value, arguments_attribute_type_>::type arguments_attribute_type;

        typedef fusion::vector3<std::string, unsigned int, unsigned int> system_exception_attribute_type;
        typedef boost::variant<system_exception_attribute_type, arguments_attribute_type> variant_attribute_type;

        typedef fusion::vector4<service_context_list, unsigned int, unsigned int
                                , variant_attribute_type>
          reply_attribute_type;
        typedef fusion::vector1<reply_attribute_type>
          message_attribute_type;
        typedef giop::grammars::arguments
          <iiop::parser_domain, iterator_type, arguments_attribute_seq_type
           , arguments_attribute_type>
          arguments_grammar;
        typedef giop::grammars::system_exception_reply_body
          <iiop::parser_domain, iterator_type, system_exception_attribute_type>
          system_exception_grammar;
        typedef giop::grammars::reply_1_0<iiop::parser_domain
                                          , iterator_type, reply_attribute_type>
          reply_grammar;
        typedef giop::grammars::message_1_0<iiop::parser_domain
                                            , iterator_type, message_attribute_type, 1u /* Reply */>
          message_grammar;
        namespace phoenix = boost::phoenix;
        morbid::arguments_traits arguments_traits;
        arguments_grammar arguments_grammar_(arguments_traits);
        system_exception_grammar system_exception_grammar_;

        reply_grammar reply_grammar_
          (
           (
            spirit::eps(phoenix::at_c<2u>(spirit::_val) == 0u)
            & arguments_grammar_
           ) |
           (
            spirit::eps(phoenix::at_c<2u>(spirit::_val) == 2u)
            & system_exception_grammar_
           )
          );

        message_grammar message_grammar_(reply_grammar_);
        namespace qi = boost::spirit::qi;
        message_attribute_type attribute;
        if(qi::parse(first, last
                     , giop::compile<iiop::parser_domain>(message_grammar_)
                     , attribute))
        {
          reply_attribute_type& reply_attr = fusion::at_c<0u>(attribute);
          std::cout << "Parsed correctly!" << std::endl;
          std::cout << "Number of service contexts " << fusion::at_c<0u>(reply_attr).size()
                    << " request id " << fusion::at_c<1u>(reply_attr)
                    << " reply status " << fusion::at_c<2u>(reply_attr) << std::endl;

          variant_attribute_type const& result = fusion::at_c<3u>(reply_attr);
          if(system_exception_attribute_type const* attribute
             = boost::get<system_exception_attribute_type>(&result))
          {
            std::cout << "Parsed system exception correctly, exception id: " << fusion::at_c<0>(*attribute)
                      << " minor code " << fusion::at_c<1>(*attribute)
                      << " completion status " << fusion::at_c<2>(*attribute) << std::endl;
          }
          else if(arguments_attribute_type const* attribute
                  = boost::get<arguments_attribute_type>(&result))
          {
            std::cout << "NO EXCEPTION" << std::endl;
            return return_value(mpl::identity<R>(), *attribute
                                , fusion::as_vector(fusion::filter_if<is_not_in_lambda>(args))
                                , mpl::identity<typename fusion::traits::tag_of<arguments_attribute_type>::type>());
          }
          else
          {
            std::cout << "Some other status" << std::endl;
          }
        }
        else
        {
          std::cout << "Failed parsing" << std::endl;
        }

        break;
      }
    }
    while(true);
  }
  else
    std::cout << "Failed generating" << std::endl;

  throw MARSHALL();
}

} }

#endif
