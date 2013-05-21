/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_GENERATE_HEADER_MODULES_HPP
#define MORBID_IDL_COMPILER_GENERATE_HEADER_MODULES_HPP

#include <morbid/idl_compiler/common_types.hpp>

#include <boost/graph/depth_first_search.hpp>

namespace morbid { namespace idl_compiler {

struct generate_header_modules_visitor : boost::default_dfs_visitor
{
  generate_header_modules_visitor(output_iterator_type& iterator)
    : iterator(iterator)
  {
  }

  void discover_vertex(vertex_descriptor v, modules_tree_type const& g);
  void finish_vertex(vertex_descriptor v, modules_tree_type const& g);

  output_iterator_type& iterator;
  std::vector<vertex_descriptor> opened_modules;
  std::vector<idl_parser::wave_string> modules_names;
};

struct generate_reference_model_visitor : boost::default_dfs_visitor
{
  generate_reference_model_visitor(output_iterator_type& iterator)
    : iterator(iterator)
  {
  }

  void discover_vertex(vertex_descriptor v, modules_tree_type const& g);
  void finish_vertex(vertex_descriptor v, modules_tree_type const& g);

  output_iterator_type& iterator;
  std::vector<vertex_descriptor> opened_modules;
  std::vector<idl_parser::wave_string> modules_names;
};

} }

#endif
