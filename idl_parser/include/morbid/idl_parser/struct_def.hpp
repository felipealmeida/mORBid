/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_STRUCT_DEF_HPP
#define MORBID_IDL_PARSER_STRUCT_DEF_HPP

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>
#include <string>

namespace morbid { namespace idl_parser {

template <typename Iterator>
struct struct_member
{
  type_spec<Iterator> type;
  std::string name;

  struct_member(type_spec<Iterator> type, std::string name)
    : type(type), name(name) {}
  struct_member() {}
};

template <typename Iterator>
struct struct_def
{
  std::string name;
  std::vector<struct_member<Iterator> > members;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, struct_member<Iterator> m)
{
  return os << "[struct_member name: " << m.name << " type: " << m.type << "]";
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, struct_def<Iterator> d)
{
  os << "[struct_def name: " << d.name << " members: ";
  std::copy(d.members.begin(), d.members.end()
            , std::ostream_iterator<struct_member<Iterator> >(os, ", "));
  return os << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::struct_member) (Iterator)
                              , (::morbid::idl_parser::type_spec<Iterator>, type)
                                (std::string, name));

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::morbid::idl_parser::struct_def) (Iterator)
                              , (std::string, name)
                                (std::vector< ::morbid::idl_parser::struct_member<Iterator> >, members));

#endif
