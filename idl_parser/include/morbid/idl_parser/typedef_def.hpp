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

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

template <typename Iterator>
struct typedef_def
{
  type_spec<Iterator> alias;
  std::string name;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, typedef_def<Iterator> d)
{
  return os << "[typedef_def name: " << d.name
            << " alias: " << d.alias
            << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::typedef_def) (Iterator)
                              , (::morbid::idl_parser::type_spec<Iterator>, alias)
                              (std::string, name));

#endif
