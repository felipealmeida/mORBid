/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_STRUCT_DEF_HPP
#define MORBID_IDL_PARSER_STRUCT_DEF_HPP

#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/member.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/wave/util/file_position.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct struct_def
{
  wave_string name;
  std::vector<member> members;
  boost::wave::util::file_position_type file_position;
};

inline std::ostream& operator<<(std::ostream& os, struct_def d)
{
  os << "[struct_def name: " << d.name << " members: ";
  std::copy(d.members.begin(), d.members.end()
            , std::ostream_iterator<member>(os, ", "));
  return os << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::struct_def
                          , (::morbid::idl_parser::wave_string, name)
                          (std::vector< ::morbid::idl_parser::member>, members)
                          (boost::wave::util::file_position_type, file_position));

#endif
