/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_OBJECT_HPP
#define MORBID_OBJECT_HPP

#include <morbid/var.hpp>
#include <morbid/string.hpp>
#include <morbid/exception.hpp>
#include <morbid/structured_ior.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid {

struct Object
{
  virtual ~Object() {} 

  virtual bool _is_a(const char* id) = 0;
  virtual structured_ior _structured_ior() const = 0;
  virtual const char* _get_interface() const = 0;

  static const char* _repository_id;
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
  Object(structured_ior const& ior)
    : _structured_ior_(ior)
  {
  }
  ~Object();

  bool _is_a(const char*);
  const char* _get_interface() const { return morbid::Object::_repository_id; }
  structured_ior _structured_ior() const { return _structured_ior_; }

  structured_ior _structured_ior_;
};

} }

#endif
