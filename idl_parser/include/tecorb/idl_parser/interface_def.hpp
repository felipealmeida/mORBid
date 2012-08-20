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
#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>

namespace tecorb { namespace idl_parser {

template <typename Iterator>
struct interface_def
{
  std::string name;
  std::vector<op_decl<Iterator> > op_decls;
  std::vector<std::string> repoids;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, interface_def<Iterator> d)
{
  return os << "[interface_def name: " << d.name << "\n"
            << "ops: " << boost::make_iterator_range(d.op_decls.begin()
                                                     , d.op_decls.end())
            << "\nrepoids: " << boost::make_iterator_range(d.repoids.begin(), d.repoids.end())
            << "]";
}

} }

BOOST_FUSION_ADAPT_TPL_STRUCT((Iterator)
                              , (::tecorb::idl_parser::interface_def) (Iterator)
                              , (std::string, name)(std::vector< ::tecorb::idl_parser::op_decl<Iterator> >, op_decls)
                              (std::vector<std::string>, repoids));

#endif
