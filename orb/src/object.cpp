/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/object.hpp>
#include <tecorb/synchronous_call.hpp>

#include <iostream>

namespace tecorb {

const char* Object::repository_id = "IDL:omg.org/CORBA/Object:1.0";

namespace remote_stub {

Object::~Object() {}

bool Object::_is_a(const char* id)
{
  using ::tecorb::synchronous_call::type_tag;
  using ::tecorb::synchronous_call::in;
  return ::tecorb::synchronous_call::call
    <bool, type_tag<char*, in> >(tecorb::Object::repository_id, "_is_a"
                                 , host, port, object_key, id);
}

} }
