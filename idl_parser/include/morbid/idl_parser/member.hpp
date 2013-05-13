/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_MEMBER_HPP
#define MORBID_IDL_PARSER_MEMBER_HPP

#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/wave_string.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct member
{
  type_spec type;
  wave_string name;

  member(type_spec type, wave_string name)
    : type(type), name(name) {}
  member() {}
};

inline std::ostream& operator<<(std::ostream& os, member m)
{
  return os << "[struct_member name: " << m.name << " type: " << m.type << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::member
                          , (::morbid::idl_parser::type_spec, type)
                          (::morbid::idl_parser::wave_string, name));

#endif
