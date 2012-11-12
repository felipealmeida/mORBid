/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/object.hpp>
#include <morbid/synchronous_call.hpp>
#include <morbid/in_out_traits.hpp>

#include <iostream>

namespace morbid {

const char* Object::repository_id = "IDL:omg.org/CORBA/Object:1.0";

namespace remote_stub {

Object::~Object() {}

bool Object::_is_a(const char* id)
{
  using ::morbid::type_tag::value_type_tag;
  using ::morbid::type_tag::in_tag;
  return ::morbid::synchronous_call::call
    <bool, value_type_tag<string, type_tag::in_tag> >
    (morbid::Object::repository_id, "_is_a"
     , host, port, object_key, id);
}

} }
