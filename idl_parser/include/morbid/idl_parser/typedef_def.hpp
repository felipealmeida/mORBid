/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_TYPEDEF_DEF_HPP
#define MORBID_IDL_PARSER_TYPEDEF_DEF_HPP

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/wave/util/file_position.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct typedef_def
{
  type_spec alias;
  wave_string name;
  boost::optional<unsigned int> array_size;
  boost::wave::util::file_position_type file_position;
};

inline std::ostream& operator<<(std::ostream& os, typedef_def d)
{
  return os << "[typedef_def name: " << d.name
            << " alias: " << d.alias
            << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::typedef_def
                          , (::morbid::idl_parser::type_spec, alias)
                          ( ::morbid::idl_parser::wave_string, name)
                          ( boost::optional<unsigned int>, array_size)
                          ( boost::wave::util::file_position_type, file_position));

#endif
