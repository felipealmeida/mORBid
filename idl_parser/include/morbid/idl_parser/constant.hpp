/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_CONSTANT_HPP
#define MORBID_IDL_PARSER_CONSTANT_HPP

#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/wave_string.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct constant
{
  type_spec type;
  wave_string name, value;

  constant(type_spec type, wave_string name, wave_string value)
    : type(type), name(name), value(value) {}
  constant() {}
};

inline std::ostream& operator<<(std::ostream& os, constant m)
{
  return os << "[constant type: " << m.type << " name: " << m.type << " = " << m.value << " ]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::constant
                          , (::morbid::idl_parser::type_spec, type)
                          (::morbid::idl_parser::wave_string, name)
                          (::morbid::idl_parser::wave_string, value));

#endif
