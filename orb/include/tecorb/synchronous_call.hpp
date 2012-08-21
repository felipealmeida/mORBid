/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef TECORB_SYNCHRONOUS_CALL_HPP
#define TECORB_SYNCHRONOUS_CALL_HPP

#include <tecorb/detail/max_args.hpp>
#include <tecorb/type_tag.hpp>
#include <tecorb/iiop/serialize_object.hpp>
#include <tecorb/iiop/generator/request_header.hpp>
#include <tecorb/iiop/generator/message_header.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/completion_condition.hpp>

#include <string>
#include <typeinfo>
#include <iostream>
#include <iterator>

namespace tecorb { namespace synchronous_call {

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TECORB_MAX_ARGS \
                                         , "tecorb/synchronous_call.hpp"))
#include BOOST_PP_ITERATE()


} }

#endif
#else
#define N() BOOST_PP_ITERATION()

#define TECORB_SYNCHRONOUS_CALL_SERIALIZE(z, n, data)                   \
  iiop::serialize_object(std::back_inserter                             \
                         (request_body), true                           \
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

  BOOST_PP_REPEAT(N(), TECORB_SYNCHRONOUS_CALL_SERIALIZE, ~)

  namespace karma = boost::spirit::karma;
  typedef std::back_insert_iterator<std::vector<char> > iterator_type;

  std::cout << "request_body size: " << request_body.size() << std::endl;
  std::vector<char> request_header_buffer;

  typedef iiop::generator::request_header<iterator_type> 
    request_header_grammar;
  request_header_grammar request_header_grammar_;
  iterator_type iterator(request_header_buffer);

  iiop::request_header request_header
    = {
        iiop::service_context_list()
        , 1u
        , true
      };
  request_header.object_key.insert
    (request_header.object_key.end(), object_key.begin(), object_key.end());
  request_header.operation.insert
    (request_header.operation.end(), method, method + std::strlen(method)+1);

  if(karma::generate(iterator, request_header_grammar_(true)
                     , request_header))
  {
    std::cout << "generated request_header" << std::endl;

    std::vector<char> message_header_buffer;
    iterator_type iterator(message_header_buffer);
    
    typedef iiop::generator::message_header<iterator_type>
      message_header_grammar;
    message_header_grammar message_header_grammar_;
    if(karma::generate(iterator, message_header_grammar_
                       (true, true, 12 + request_header_buffer.size()
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

#endif
