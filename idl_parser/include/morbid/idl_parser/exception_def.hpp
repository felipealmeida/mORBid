/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_EXCEPTION_DEF_HPP
#define MORBID_IDL_PARSER_EXCEPTION_DEF_HPP

#include <morbid/idl_parser/wave_string.hpp>
#include <morbid/idl_parser/member.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

struct exception_def
{
  wave_string name;
  std::vector<std::vector<member> > members;
};

inline std::ostream& operator<<(std::ostream& os, exception_def d)
{
  return os << "[exception_def name: " << d.name << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT(::morbid::idl_parser::exception_def
                          , (::morbid::idl_parser::wave_string, name)
                          (std::vector<std::vector< ::morbid::idl_parser::member> >, members));

#endif
