/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_STRUCT_DEF_HPP
#define MORBID_IDL_PARSER_STRUCT_DEF_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

template <typename Iterator>
struct struct_def
{
  std::string name;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, struct_def<Iterator> d)
{
  return os << "[struct_def name: " << d.name << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::struct_def) (Iterator)
                              , (std::string, name));

#endif
