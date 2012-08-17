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

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <string>
#include <typeinfo>
#include <iostream>

namespace tecorb { namespace synchronous_call {

struct in {};
struct out {};
struct inout {};

template <typename T>
struct in_type_traits
{
  typedef T type;
};

template <>
struct in_type_traits<char*>
{
  typedef const char* type;
};

template <typename T, typename Tag>
struct type_tag;

template <typename T>
struct type_tag<T, in>
{
  typedef T type;
  typedef in tag;

  type_tag(typename in_type_traits<type>::type value) : value(value) {}

  typename in_type_traits<type>::type value;
};

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TECORB_MAX_ARGS \
                                         , "tecorb/synchronous_call.hpp"))
#include BOOST_PP_ITERATE()


} }

#endif
#else
#define N() BOOST_PP_ITERATION()

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

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::endpoint::protocol_type::v4(), host, "");
  boost::asio::ip::tcp::endpoint remote_endpoint = *resolver.resolve(query);
  remote_endpoint.port(port);
  boost::asio::ip::tcp::socket socket(io_service, boost::asio::ip::tcp::endpoint());
  socket.connect(remote_endpoint);

  

  return R();
}

#endif
