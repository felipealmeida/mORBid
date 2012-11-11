/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef TECORB_SYNCHRONOUS_CALL_HPP
#define TECORB_SYNCHRONOUS_CALL_HPP

#include <morbid/detail/max_args.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/parse_argument.hpp>
#include <morbid/iiop/serialize_object.hpp>
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

inline
void reply_return(std::vector<char>::iterator begin, std::vector<char>::iterator first
               , std::vector<char>::iterator last, bool little_endian
               , morbid::argument_tag<void>)
{
  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
  iiop::reply_header reply_header;
  iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
  if(qi::parse(first, last, reply_header_grammar(phoenix::val(begin), little_endian), reply_header))
  {
    std::cout << "Parsed successfully but no return type" << std::endl;
    return;
  }
  else
    throw std::runtime_error("Failed parsing reply header");
}

template <typename R>
R reply_return(std::vector<char>::iterator begin, std::vector<char>::iterator first
               , std::vector<char>::iterator last, bool little_endian
               , morbid::argument_tag<R>)
{
  namespace qi = boost::spirit::qi;
  namespace phoenix = boost::phoenix;
  iiop::reply_header reply_header;
  iiop::grammar::reply_header_1_1<std::vector<char>::iterator> reply_header_grammar;
  if(qi::parse(first, last, reply_header_grammar(phoenix::val(first), little_endian), reply_header))
  {
    std::cout << "Reply header parsed" << std::endl;
    const char* first_ = &*first;
    return parse_argument(&*begin, first_
                          , boost::next(&*begin, std::distance(begin, last))
                          , little_endian
                          , morbid::argument_tag<R>());
  }
  else
    throw std::runtime_error("Failed parsing reply header");
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TECORB_MAX_ARGS \
                                         , "morbid/synchronous_call.hpp"))
#include BOOST_PP_ITERATE()


} }

#endif
#else
#define N() BOOST_PP_ITERATION()

#define TECORB_SYNCHRONOUS_CALL_SERIALIZE(z, n, data)                   \
  iiop::serialize_object(iterator, true                                 \
                         , BOOST_PP_CAT(a, n).value);                       

template <typename R BOOST_PP_ENUM_TRAILING_PARAMS(N(), typename A)>
R call(const char* repoid, const char* method
       , std::string const& host, unsigned short port
       , std::string const& object_key
       BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N(), A, a))
{
  std::cout << "Making synchronous call to " << host << ":" << port
            << " to object with object_key " << object_key
            << " with repoid: " << repoid << " to method " << method
            << " and signature " << typeid(R(BOOST_PP_ENUM_PARAMS(N(), A))).name()
            << std::endl;

  std::vector<char> request_body;

  {
    std::back_insert_iterator<std::vector<char> > iterator(request_body);

    BOOST_PP_REPEAT(N(), TECORB_SYNCHRONOUS_CALL_SERIALIZE, ~)
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
        return synchronous_call::reply_return(reply_buffer.begin(), first
                                              , reply_buffer.end(), little_endian
                                              , argument_tag<R>());
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
#undef N
#endif
