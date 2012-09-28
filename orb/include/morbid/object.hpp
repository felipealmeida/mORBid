/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_OBJECT_HPP
#define TECORB_OBJECT_HPP

#include <morbid/var.hpp>
#include <morbid/string.hpp>
#include <morbid/exception.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid {

struct Object
{
  virtual ~Object() {} 

  virtual String_ptr ior() const = 0;
  virtual bool _is_a(const char* id) = 0;
  virtual std::string const& _host() const { std::abort(); }
  virtual std::string const& _objectkey() const { std::abort(); }
  virtual unsigned short _port() const { std::abort(); }

  static const char* repository_id;
};

typedef boost::shared_ptr<Object> Object_ptr;
typedef morbid::var<Object> Object_var;

template <typename T>
bool is_nil(boost::shared_ptr<T> obj)
{
  return !obj;
}

template <typename T>
bool is_nil(var<T> obj)
{
  return !obj;
}

namespace remote_stub {

struct Object : morbid::Object
{
  Object(std::string const& host, unsigned short port
         , std::string object_key)
    : host(host), port(port), object_key(object_key)
  {
  }
  ~Object();

  bool _is_a(const char*);

  std::string const& _host() const { return host; }
  std::string const& _objectkey() const { return object_key; }
  unsigned short _port() const { return port; }

  String_ptr ior() const
  {
    namespace karma = boost::spirit::karma;
    std::string string;
    karma::generate(std::back_inserter<std::string>(string)
                    , "corbaloc::" << karma::lit(host)
                    << ":" << karma::ushort_(port)
                    << "/" << karma::lit(object_key));
    String_ptr r( new char[string.size()+1] );
    std::strcpy(r.get(), string.c_str());
    return r;
  }

  std::string host;
  unsigned short port;
  std::string object_key;
};

} }

#endif
