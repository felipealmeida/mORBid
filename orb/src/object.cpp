/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/object.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/synchronous_call.hpp>
#include <morbid/in_out_traits.hpp>

#include <boost/fusion/include/make_vector.hpp>

#include <iostream>

namespace morbid {

const char* Object::_repository_id = "IDL:omg.org/CORBA/Object:1.0";

namespace remote_stub {

Object::~Object() {}

bool Object::_is_a(const char* id)
{
  std::cout << "Object::_is_a " << id << std::endl;
  using ::morbid::type_tag::value_type_tag;
  using ::morbid::type_tag::in_tag;
  return ::morbid::synchronous_call::call<morbid::Boolean>
    (morbid::Object::_repository_id, "_is_a"
     , _structured_ior_
     , boost::fusion::vector<value_type_tag<string, type_tag::in_tag> >(id));
}

} }
