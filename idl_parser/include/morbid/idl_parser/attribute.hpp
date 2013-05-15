/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_ATTRIBUTE_HPP
#define MORBID_IDL_PARSER_ATTRIBUTE_HPP

#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/wave_string.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/wave/util/file_position.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct attribute
{
  bool readonly;
  type_spec type;
  wave_string name;
  boost::wave::util::file_position_type file_position;
};

inline std::ostream& operator<<(std::ostream& os, attribute m)
{
  return os << "[attribute name: " << m.name << " type: " << m.type << " readonly: " << m.readonly << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::attribute
                          , 
                          (bool, readonly)
                          (::morbid::idl_parser::type_spec, type)
                          (::morbid::idl_parser::wave_string, name)
                          (::boost::wave::util::file_position_type, file_position));

#endif
