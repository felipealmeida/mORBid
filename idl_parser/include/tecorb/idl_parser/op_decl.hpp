/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_OP_DECL_HPP
#define TECORB_IDL_PARSER_OP_DECL_HPP

#include <boost/fusion/include/adapt_struct.hpp>

namespace tecorb { namespace idl_parser {

template <typename Iterator>
struct op_decl
{
  typedef typename Iterator::base_iterator_type base_iterator;

  op_decl() {}
  op_decl(boost::iterator_range<base_iterator> type
          , boost::iterator_range<base_iterator> name)
    : type(type.begin(), type.end()), name(name.begin(), name.end()) {}

  std::string type, name;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, op_decl<Iterator> op)
{
  return os << "[op_decl type: " << op.type << " name: " << op.name << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::tecorb::idl_parser::op_decl) (Iterator)
                              , (std::string, type)(std::string, name)
                              );

#endif
