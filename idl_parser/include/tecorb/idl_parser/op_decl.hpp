/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_OP_DECL_HPP
#define TECORB_IDL_PARSER_OP_DECL_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/range/iterator_range.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace tecorb { namespace idl_parser {

struct param_decl
{
  std::string direction;
  std::string type;
};

template <typename Iterator>
struct op_decl
{
  typedef typename Iterator::base_iterator_type base_iterator;

  std::string type, name;
  std::vector<param_decl> params;
  bool user_defined;
};

inline std::ostream& operator<<(std::ostream& os, param_decl op)
{
  return os << "[param_decl direction: " << op.direction << " type: " << op.type << "]";
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, op_decl<Iterator> op)
{
  return os << "[op_decl type: " << op.type << " name: " << op.name << " user_defined: " << op.user_defined
            << " params: " << boost::make_iterator_range(op.params.begin(), op.params.end()) << "]";
}

} }

BOOST_FUSION_ADAPT_STRUCT( ::tecorb::idl_parser::param_decl
                           , (std::string, direction)(std::string, type)
                          );

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::tecorb::idl_parser::op_decl) (Iterator)
                              , (std::string, type)(std::string, name)
                              (std::vector< ::tecorb::idl_parser::param_decl>, params)
                              (bool, user_defined)
                              );

#endif
