/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

// #if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef MORBID_SYNCHRONOUS_CALL_HPP
#define MORBID_SYNCHRONOUS_CALL_HPP

#include <morbid/detail/max_args.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/structured_ior.hpp>
#include <morbid/struct_fusion.hpp>
#include <morbid/giop/grammars/message_1_0.hpp>
#include <morbid/giop/grammars/request_1_0.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
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

// namespace process_reply_detail {

// template <typename OutputMPLSeq, typename OutputArgsSeq>
// inline void process_reply(std::vector<char>::iterator vbegin, std::vector<char>::iterator vfirst
//                           , std::vector<char>::iterator vlast, bool little_endian
//                           , mpl::identity<void>
//                           , OutputArgsSeq output_args)
// {
//   // namespace qi = boost::spirit::qi;
//   namespace phoenix = boost::phoenix;
//   namespace mpl = boost::mpl;

//   iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
//   iiop::reply_header reply_header;

//   std::cout << "not_in_params read_arguments " << typeid(output_args).name() << std::endl;
//   std::cout << "Args " << boost::fusion::result_of::size<OutputArgsSeq>::value << std::endl;

//   typedef typename boost::fusion::result_of::push_front
//     <OutputArgsSeq const
//      , iiop::reply_header&>::type
//     sequence_type;
//   sequence_type sequence = boost::fusion::push_front
//     (output_args, boost::ref(reply_header));

//   BOOST_MPL_ASSERT((mpl::if_c<mpl::size<sequence_type>::value == 1
//                     , mpl::bool_<mpl::distance<typename mpl::next<typename mpl::begin<sequence_type>::type>::type
//                     , typename boost::mpl::end<sequence_type>::type>::value == 0>
//                     , mpl::true_>));

//   return create_expression_and_parse
//     <typename mpl::begin<OutputMPLSeq>::type
//      , typename mpl::end<OutputMPLSeq>::type
//      , mpl::bool_<boost::fusion::result_of::size<sequence_type>::value == 1> >
//     ::template call<void>
//     (vbegin, vfirst, vlast, little_endian, mpl::true_(), sequence
//      , reply_header_grammar(phoenix::val(vbegin), little_endian));
// }

// template <typename OutputMPLSeq, typename R, typename OutputArgsSeq>
// inline typename return_traits<R>::type
//   process_reply(std::vector<char>::iterator vbegin, std::vector<char>::iterator vfirst
//                 , std::vector<char>::iterator vlast, bool little_endian
//                 , mpl::identity<R>
//                 , OutputArgsSeq output_args)
// {
//   // namespace qi = boost::spirit::qi;
//   namespace phoenix = boost::phoenix;
//   namespace mpl = boost::mpl;

//   iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
//   iiop::reply_header reply_header;

//   std::cout << "not_in_params read_arguments " << typeid(output_args).name() << std::endl;
//   std::cout << "Args " << boost::fusion::result_of::size<OutputArgsSeq>::value << std::endl;

//   typedef typename return_traits<R>::type result_type;
//   result_type r = result_type();
//   typedef typename boost::fusion::result_of::push_front
//     <typename boost::add_const<typename boost::fusion::result_of::push_back
//                               <OutputArgsSeq const, result_type&>::type>::type
//       , iiop::reply_header&>::type
//     sequence_type;
//   sequence_type sequence = boost::fusion::push_front
//     (boost::fusion::push_back(output_args, boost::ref(r)), boost::ref(reply_header));

//   typedef typename mpl::push_back<OutputMPLSeq, R>::type mpl_sequence;

//   return create_expression_and_parse
//     <typename mpl::begin<mpl_sequence>::type
//      , typename mpl::end<mpl_sequence>::type
//      , boost::mpl::false_>
//     ::template call<typename return_traits<R>::type>
//     (vbegin, vfirst, vlast, little_endian, mpl::false_(), sequence
//      , reply_header_grammar(phoenix::val(vbegin), little_endian));
// }

// struct remove_value_type;

// template <typename F>
// struct remove_value_type_result_of;

// template <typename A>
// struct remove_value_type_result_of<remove_value_type(A)>
// {
//   typedef typename boost::remove_const
//     <typename boost::remove_reference<A>::type>::type::type type;
// };

// struct remove_value_type
// {
//   template <typename F>
//   struct result : remove_value_type_result_of<F> {};

//   template <typename A>
//   typename result<remove_value_type(A)>::type
//   operator()(A a) const
//   {
//     return a.value;
//   }
// };

// }

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

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;

  typedef std::vector<fusion::vector2<unsigned int, std::vector<char> > > service_context_list;
  typedef fusion::vector7<service_context_list
                          , unsigned int, bool, std::vector<char>, std::string, std::vector<char>
                          , ArgsSeq const&>
    request_attribute_type;
  typedef fusion::vector3<char, char, fusion::vector2<char, request_attribute_type> >
    message_attribute_type;

  typedef giop::grammars::request_1_0<iiop::generator_domain
                                      , output_iterator_type, request_attribute_type>
    request_header_grammar;
  typedef giop::grammars::message_1_0<iiop::generator_domain
                                      , output_iterator_type, message_attribute_type>
    message_header_grammar;

  request_header_grammar request_header_grammar_;
  message_header_grammar message_header_grammar_(request_header_grammar_);
  message_attribute_type attribute ('\1', '\0'
                                    , fusion::make_vector
                                    ('\0' 
                                     , request_attribute_type
                                     (service_context_list(), 1u, true, std::vector<char>()
                                      , method, std::vector<char>(), args)));
  
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
        std::cout << "Should try to parse" << std::endl;
        break;
      }
    }
    while(true);
  }
  else
    std::cout << "Failed generating" << std::endl;

  // std::vector<char> request_body;
  // {
  //   typedef std::back_insert_iterator<std::vector<char> > iterator_type;
  //   iterator_type iterator(request_body);
  //   process_reply_detail::serialize_if_input<iterator_type> serialize(iterator);
  //   boost::fusion::for_each(args, serialize);
  // }

  // namespace karma = boost::spirit::karma;
  // typedef std::back_insert_iterator<std::vector<char> > iterator_base_type;
  // typedef iterator_base_type iterator_type;

  // std::cout << "request_body size: " << request_body.size() << std::endl;
  // std::vector<char> request_header_buffer;

  // typedef iiop::generator::request_header<iterator_type> 
  //   request_header_grammar;
  // request_header_grammar request_header_grammar_;
  // iterator_type iterator(request_header_buffer);


  // /* lets try here */

  // {
  //   typedef typename mpl::copy_if<ArgsSeq
  //                                 , type_tag::is_not_in_type_tag<mpl::_1>
  //                                 , mpl::back_inserter<mpl::vector0<> >
  //                                 >::type
  //     not_in_arguments_type;

  //   iiop::iiop_grammar_builder
  //     <karma::domain, iterator_type, not_in_arguments_type, iiop::primitive_traits>
  //     iiop_arguments_grammar;


  //   (void)iiop_arguments_grammar;
  //     // , boost::fusion::transform
  //     // (
  //     //  boost::fusion::filter_if<typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type>(args)
  //     //  , process_reply_detail::remove_value_type()
  //     // )
  // }

  // /*****************/

  // iiop::request_header request_header
  //   = {
  //       iiop::service_context_list()
  //       , 1u
  //       , true
  //     };
  // request_header.object_key.insert
  //   (request_header.object_key.end(), object_key.begin(), object_key.end());
  // request_header.operation.insert
  //   (request_header.operation.end(), method, method + std::strlen(method));

  // if(karma::generate(iterator, request_header_grammar_(true), request_header))
  // {
  //   std::cout << "generated request_header" << std::endl;

  //   std::vector<char> message_header_buffer;
  //   iterator_type iterator(message_header_buffer);
    
  //   typedef iiop::generator::message_header<iterator_type>
  //     message_header_grammar;
  //   message_header_grammar message_header_grammar_;
  //   if(karma::generate(iterator, message_header_grammar_
  //                      (true, 0u /*request*/, 12 + request_header_buffer.size()
  //                       + request_body.size())))
  //   {
  //     std::cout << "Generated message_header" << std::endl;
  //     {
  //       boost::asio::io_service io_service;
  //       boost::asio::ip::tcp::resolver resolver(io_service);
  //       boost::asio::ip::tcp::resolver::query query
  //         (boost::asio::ip::tcp::endpoint::protocol_type::v4(), host, "");
  //       boost::asio::ip::tcp::endpoint remote_endpoint = *resolver.resolve(query);
  //       remote_endpoint.port(port);
  //       boost::asio::ip::tcp::socket socket(io_service, boost::asio::ip::tcp::endpoint());
  //       socket.connect(remote_endpoint);
  //       boost::asio::write(socket, boost::asio::buffer(message_header_buffer)
  //                          , boost::asio::transfer_all());
  //       boost::asio::write(socket, boost::asio::buffer(request_header_buffer)
  //                          , boost::asio::transfer_all());
  //       boost::asio::write(socket, boost::asio::buffer(request_body)
  //                          , boost::asio::transfer_all());

  //       // namespace qi = boost::spirit::qi;

  //       std::vector<char> reply_buffer(1024u);
  //       std::size_t offset = 0;
  //       std::vector<char>::iterator first;
  //       bool little_endian;
  //       do
  //       {
  //         if(reply_buffer.size() - offset < 128u)
  //           reply_buffer.resize(reply_buffer.size() + 1024u);
  //         boost::system::error_code ec;
  //         std::size_t bytes_read
  //           = socket.read_some(boost::asio::buffer(&reply_buffer[0], reply_buffer.size() - offset), ec);
  //         std::cout << "Read " << bytes_read << " bytes" << std::endl;
  //         if(ec)
  //           throw ec;
  //         else
  //         {
  //           iiop::message_header message_header;
  //           iiop::grammar::message_header<std::vector<char>::iterator> message_header_grammar;
  //           offset += bytes_read;
  //           std::vector<char>::iterator last = boost::next(reply_buffer.begin(), offset);
  //           first = reply_buffer.begin();
  //           if(qi::parse(first, last, message_header_grammar, message_header))
  //           {
  //             unsigned char size_tmp = message_header.message_size;
  //             int size = size_tmp;
  //             if(std::distance(first, last) >= size)
  //             {
  //               std::cout << "All message received" << std::endl;
  //               if(message_header.message_type == 1 /* reply */)
  //               {
  //                 little_endian = message_header.flags & 1;
  //                 reply_buffer.erase(last, reply_buffer.end());
  //                 break;
  //               }
  //               else
  //                 throw std::runtime_error("Error, response not what we expected");
  //             }
  //             else
  //             {
  //               std::cout << "Continue reading" << std::endl;
  //             }
  //           }
  //           else if(std::distance(first, last) >= 12)
  //           {
  //             std::cout << "Failed reading GIOP" << std::endl;
  //             throw std::runtime_error("Failed reading GIOP reply");
  //           }
  //         }
  //       }
  //       while(true);

  //       std::cout << "Process reply" << std::endl;
  //       return process_reply_detail::process_reply
  //         <typename mpl::transform
  //          <
  //            typename mpl::copy_if
  //            <ArgsSeq
  //             , type_tag::is_not_in_type_tag<mpl::_1>
  //             , mpl::back_inserter<mpl::vector0<> >
  //            >::type
  //            , type_tag::original_type<mpl::_1>
  //          >::type
  //         >
  //         (reply_buffer.begin(), first
  //          , reply_buffer.end(), little_endian
  //          , mpl::identity<R>()
  //          , boost::fusion::transform
  //           (
  //              boost::fusion::filter_if<typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type>(args)
  //            , process_reply_detail::remove_value_type()
  //           )
  //         );
  //     }      
  //   }
  //   else
  //   {
  //     std::cout << "*NOT* Generated message_header" << std::endl;
  //   }
  // }
  // else
  // {
  //   std::cout << "Failed generating request_header" << std::endl;
  // }
  throw MARSHALL();
}

} }

#endif
