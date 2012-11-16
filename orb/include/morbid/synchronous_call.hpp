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
#include <morbid/parse_argument.hpp>
#include <morbid/serialize_object.hpp>
#include <morbid/struct_fusion.hpp>
#include <morbid/iiop/generator/request_header.hpp>
#include <morbid/iiop/generator/message_header.hpp>
#include <morbid/iiop/message_header.hpp>
#include <morbid/iiop/grammar/message_header.hpp>
#include <morbid/iiop/grammar/reply_header_1_1.hpp>

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
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/remove_if.hpp>

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

namespace process_reply_detail {

template <typename Iterator>
struct serialize_if_input
{
  serialize_if_input(Iterator& iterator)
    : iterator(iterator) {}

  template <typename T, typename Tag>
  void operator()(type_tag::value_type_tag<T, Tag> a) const
  {
    serialization::serialize_object(iterator, a.value);
  }
  template <typename T>
  void operator()(type_tag::value_type_tag<T, type_tag::out_tag>) const
  {
  }

  Iterator& iterator;
};

template <typename Iter, typename Last, typename SequenceSizeIsOne, typename Enabler = void>
struct create_expression_and_parse;

namespace qi = boost::spirit::qi;

template <typename Iter, typename Last>
struct create_expression_and_parse<Iter, Last, mpl::false_
                                   , typename boost::disable_if_c
                                   <mpl::distance<Iter, Last>::value == 0
                                    , void>::type>
{
  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Boolean>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    qi::rule<Iterator, morbid::Boolean()> r = qi::as<morbid::Boolean>()[qi::char_];
    r.name("boolean");
    qi::debug(r);
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> r);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Char>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::char_);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Octet>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::char_);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Float>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> iiop::grammar::float_<Iterator>()(little_endian));
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Double>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> iiop::grammar::double_<Iterator>()(little_endian));
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::UShort>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::UShort>()[iiop::grammar::word<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Short>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::Short>()[iiop::grammar::word<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::ULong>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::ULong>()[iiop::grammar::dword<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::Long>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::Long>()[iiop::grammar::dword<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::ULongLong>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::ULongLong>()[iiop::grammar::qword<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<morbid::LongLong>
                                 , OutputSeq seq, ParserExpr const& expr)
  {
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> qi::as<morbid::LongLong>()[iiop::grammar::qword<Iterator>()(little_endian)]);
  }

  template <typename R, typename Iterator, typename IsVoid, typename T, typename OutputSeq, typename ParserExpr>
  inline static R create_grammar(Iterator vbegin, Iterator vfirst
                                 , Iterator vlast, bool little_endian
                                 , IsVoid is_void, argument_tag<T>
                                 , OutputSeq seq, ParserExpr const& expr
                                 , typename boost::enable_if
                                 <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                                 , void*>::type = 0)
  {
    typename T::template _morbid_parser<Iterator> parser;
    return create_expression_and_parse<typename boost::mpl::next<Iter>::type, Last, boost::mpl::false_>
      ::template call<R>(vbegin, vfirst, vlast, little_endian, is_void, seq
                         , expr >> parser);
  }

  template <typename R, typename Iterator, typename IsVoid, typename OutputSeq, typename ParserExpr>
  inline static R call(Iterator vbegin, Iterator vfirst
                       , Iterator vlast, bool little_endian
                       , IsVoid is_void, OutputSeq seq, ParserExpr const& expr)
  {
    return create_grammar<R>(vbegin, vfirst, vlast, little_endian, is_void
                             , argument_tag
                             <typename boost::remove_reference
                             <typename boost::mpl::deref<Iter>::type
                             >::type>()
                             , seq, expr);
  }
};

template <typename Iter, typename Last>
struct create_expression_and_parse<Iter, Last, boost::mpl::false_
                                   , typename boost::enable_if_c
                                   <mpl::distance<Iter, Last>::value == 0
                                    , void>::type>
{
  template <typename R, typename Iterator, typename OutputSeq, typename ParserExpr>
  inline static R call(Iterator vbegin, Iterator vfirst
                       , Iterator vlast, bool little_endian
                       , boost::mpl::false_ // not void
                       , OutputSeq seq, ParserExpr const& expr)
  {
    BOOST_MPL_ASSERT((mpl::not_<typename boost::is_const<OutputSeq>::type>));
    namespace qi = boost::spirit::qi;
    boost::fusion::front(seq).request_id = 5;
    if(qi::parse(vfirst, vlast, expr, seq))
    {
      std::cout << "Parsed successfully" << std::endl;
      std::cout << "request id " << boost::fusion::front(seq).request_id << std::endl;
      std::cout << "Return value " << boost::fusion::back(seq) << std::endl;
      return boost::fusion::back(seq);
    }
    else
      throw morbid::MARSHALL();
  }

  template <typename R, typename Iterator, typename OutputSeq, typename ParserExpr>
  inline static R call(Iterator vbegin, Iterator vfirst
                       , Iterator vlast, bool little_endian
                       , boost::mpl::true_ // is void
                       , OutputSeq seq, ParserExpr const& expr)
  {
    BOOST_MPL_ASSERT((mpl::not_<boost::is_const<OutputSeq> >));
    namespace qi = boost::spirit::qi;
    boost::fusion::front(seq).request_id = 5;
    if(qi::parse(vfirst, vlast, expr, seq))
    {
      std::cout << "Parsed successfully but no return type" << std::endl;
      std::cout << "request id " << boost::fusion::front(seq).request_id << std::endl;
      return;
    }
    else
      throw morbid::MARSHALL();
  }
};

template <typename Iter, typename Last>
struct create_expression_and_parse<Iter, Last, boost::mpl::true_ // sequence size is one. Must not use sequence as attribute
                                   , typename boost::enable_if_c
                                   <mpl::distance<Iter, Last>::value == 0
                                    , void>::type>
{
  template <typename R, typename Iterator, typename OutputSeq, typename ParserExpr>
  inline static R call(Iterator vbegin, Iterator vfirst
                       , Iterator vlast, bool little_endian
                       , boost::mpl::true_ // is void
                       , OutputSeq seq, ParserExpr const& expr)
  {
    namespace qi = boost::spirit::qi;
    iiop::reply_header& reply_header = boost::fusion::front(seq);
    if(qi::parse(vfirst, vlast, expr, reply_header))
    {
      std::cout << "Parsed successfully but no return type" << std::endl;
      std::cout << "request id " << boost::fusion::front(seq).request_id << std::endl;
      return;
    }
    else
      throw morbid::MARSHALL();
  }
};

template <typename OutputArgsSeq>
inline void process_reply(std::vector<char>::iterator vbegin, std::vector<char>::iterator vfirst
                          , std::vector<char>::iterator vlast, bool little_endian
                          , morbid::argument_tag<void>
                          , OutputArgsSeq output_args)
{
  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
  namespace mpl = boost::mpl;

  iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
  iiop::reply_header reply_header;

  std::cout << "not_in_params read_arguments " << typeid(output_args).name() << std::endl;
  std::cout << "Args " << boost::fusion::result_of::size<OutputArgsSeq>::value << std::endl;

  typedef typename boost::fusion::result_of::push_front
    <OutputArgsSeq const
     , iiop::reply_header&>::type
    sequence_type;
  sequence_type sequence = boost::fusion::push_front
    (output_args, boost::ref(reply_header));

  BOOST_MPL_ASSERT((mpl::if_c<mpl::size<sequence_type>::value == 1
                    , mpl::bool_<mpl::distance<typename mpl::next<typename mpl::begin<sequence_type>::type>::type
                    , typename boost::mpl::end<sequence_type>::type>::value == 0>
                    , mpl::true_>));

  return create_expression_and_parse
    <typename mpl::next<typename mpl::begin<sequence_type>::type>::type
     , typename mpl::end<sequence_type>::type
     , mpl::bool_<boost::fusion::result_of::size<sequence_type>::value == 1> >
    ::template call<void>(vbegin, vfirst, vlast, little_endian, mpl::true_(), sequence
                          , reply_header_grammar(phoenix::val(vbegin), little_endian));
}

template <typename R, typename OutputArgsSeq>
inline R process_reply(std::vector<char>::iterator vbegin, std::vector<char>::iterator vfirst
                       , std::vector<char>::iterator vlast, bool little_endian
                       , morbid::argument_tag<R>
                       , OutputArgsSeq output_args)
{
  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
  namespace mpl = boost::mpl;

  iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
  iiop::reply_header reply_header;

  std::cout << "not_in_params read_arguments " << typeid(output_args).name() << std::endl;
  std::cout << "Args " << boost::fusion::result_of::size<OutputArgsSeq>::value << std::endl;

  R r = R();
  typedef typename boost::fusion::result_of::push_front
    <typename boost::add_const<typename boost::fusion::result_of::push_back
                              <OutputArgsSeq const, R&>::type>::type
      , iiop::reply_header&>::type
    sequence_type;
  sequence_type sequence = boost::fusion::push_front
    (boost::fusion::push_back(output_args, boost::ref(r)), boost::ref(reply_header));

  return create_expression_and_parse
    <typename boost::mpl::next<typename boost::mpl::begin<sequence_type>::type>::type
     , typename boost::mpl::end<sequence_type>::type
     , boost::mpl::false_>
    ::template call<R>(vbegin, vfirst, vlast, little_endian, mpl::false_(), sequence
                       , reply_header_grammar(phoenix::val(vbegin), little_endian));
}

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

}

template <typename R, typename ArgsSeq>
R call(const char* repoid, const char* method, std::string const& host, unsigned short port
       , std::string const& object_key, ArgsSeq args)
{
  namespace mpl = boost::mpl;
  std::cout << "Making synchronous call to " << host << ":" << port
            << " to object with object_key " << object_key
            << " with repoid: " << repoid << " to method " << method
            << " and args [" << typeid(args).name()
            << ']' << std::endl;

  std::vector<char> request_body;

  {
    typedef std::back_insert_iterator<std::vector<char> > iterator_type;
    iterator_type iterator(request_body);
    process_reply_detail::serialize_if_input<iterator_type> serialize(iterator);
    boost::fusion::for_each(args, serialize);
  }

  namespace karma = boost::spirit::karma;
  typedef std::back_insert_iterator<std::vector<char> > iterator_base_type;
  typedef iterator_base_type iterator_type;

  std::cout << "request_body size: " << request_body.size() << std::endl;
  std::vector<char> request_header_buffer;

  typedef iiop::generator::request_header<iterator_type> 
    request_header_grammar;
  request_header_grammar request_header_grammar_;
  iterator_base_type iterator_base(request_header_buffer);
  iterator_type iterator(iterator_base);

  iiop::request_header request_header
    = {
        iiop::service_context_list()
        , 1u
        , true
      };
  request_header.object_key.insert
    (request_header.object_key.end(), object_key.begin(), object_key.end());
  request_header.operation.insert
    (request_header.operation.end(), method, method + std::strlen(method));

  if(karma::generate(iterator, request_header_grammar_(true), request_header))
  {
    std::cout << "generated request_header" << std::endl;

    std::vector<char> message_header_buffer;
    iterator_base_type iterator_base(message_header_buffer);
    iterator_type iterator(iterator_base);
    
    typedef iiop::generator::message_header<iterator_type>
      message_header_grammar;
    message_header_grammar message_header_grammar_;
    if(karma::generate(iterator, message_header_grammar_
                       (true, 0u /*request*/, 12 + request_header_buffer.size()
                        + request_body.size())))
    {
      std::cout << "Generated message_header" << std::endl;
      {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query
          (boost::asio::ip::tcp::endpoint::protocol_type::v4(), host, "");
        boost::asio::ip::tcp::endpoint remote_endpoint = *resolver.resolve(query);
        remote_endpoint.port(port);
        boost::asio::ip::tcp::socket socket(io_service, boost::asio::ip::tcp::endpoint());
        socket.connect(remote_endpoint);
        boost::asio::write(socket, boost::asio::buffer(message_header_buffer)
                           , boost::asio::transfer_all());
        boost::asio::write(socket, boost::asio::buffer(request_header_buffer)
                           , boost::asio::transfer_all());
        boost::asio::write(socket, boost::asio::buffer(request_body)
                           , boost::asio::transfer_all());

        namespace qi = boost::spirit::qi;

        std::vector<char> reply_buffer(1024u);
        std::size_t offset = 0;
        std::vector<char>::iterator first;
        bool little_endian;
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
            iiop::message_header message_header;
            iiop::grammar::message_header<std::vector<char>::iterator> message_header_grammar;
            offset += bytes_read;
            std::vector<char>::iterator last = boost::next(reply_buffer.begin(), offset);
            first = reply_buffer.begin();
            if(qi::parse(first, last, message_header_grammar, message_header))
            {
              unsigned char size_tmp = message_header.message_size;
              int size = size_tmp;
              if(std::distance(first, last) >= size)
              {
                std::cout << "All message received" << std::endl;
                if(message_header.message_type == 1 /* reply */)
                {
                  little_endian = message_header.flags & 1;
                  reply_buffer.erase(last, reply_buffer.end());
                  break;
                }
                else
                  throw std::runtime_error("Error, response not what we expected");
              }
              else
              {
                std::cout << "Continue reading" << std::endl;
              }
            }
            else if(std::distance(first, last) >= 12)
            {
              std::cout << "Failed reading GIOP" << std::endl;
              throw std::runtime_error("Failed reading GIOP reply");
            }
          }
        }
        while(true);

        std::cout << "Process reply" << std::endl;
        return process_reply_detail::process_reply
          (reply_buffer.begin(), first
           , reply_buffer.end(), little_endian
           , argument_tag<R>()
           , boost::fusion::transform
            (
               boost::fusion::filter_if<typename mpl::lambda<type_tag::is_not_in_type_tag<mpl::_1> >::type>(args)
             , process_reply_detail::remove_value_type()
            )
          );
      }      
    }
    else
    {
      std::cout << "*NOT* Generated message_header" << std::endl;
    }
  }
  else
  {
    std::cout << "Failed generating request_header" << std::endl;
  }

  return R();
}

} }

#endif
