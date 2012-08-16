/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_INTERFACE_DEF_HPP
#define TECORB_IDL_PARSER_INTERFACE_DEF_HPP

#include <tecorb/idl_parser/op_decl.hpp>

#include <boost/range/iterator_range.hpp>

#include <ostream>

namespace tecorb { namespace idl_parser {

template <typename Iterator>
struct interface_def
{
  typedef typename Iterator::base_iterator_type base_iterator;

  interface_def() {}
  interface_def(boost::iterator_range<base_iterator> name
                , std::vector<op_decl<Iterator> > op_decls)
    : name(name), op_decls(op_decls) {}

  boost::iterator_range<base_iterator> name;
  std::vector<op_decl<Iterator> > op_decls;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, interface_def<Iterator> d)
{
  return os << "[interface_def name: " << d.name << "\n"
            << "ops: " << boost::make_iterator_range(d.op_decls.begin()
                                                     , d.op_decls.end())
            << "]";
}

} }

#endif
