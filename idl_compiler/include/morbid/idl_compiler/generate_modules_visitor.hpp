/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_GENERATE_MODULES_HPP
#define MORBID_IDL_COMPILER_GENERATE_MODULES_HPP

#include <morbid/idl_compiler/common_types.hpp>

#include <boost/graph/breadth_first_search.hpp>

namespace morbid { namespace idl_compiler {

struct generate_modules_visitor : boost::default_bfs_visitor
{
  generate_modules_visitor()
    : state(new shared_state)
  {}

  struct shared_state
  {
    std::vector<vertex_descriptor> opened_modules;
  };

  void examine_vertex(vertex_descriptor v, modules_tree_type const& g);
  // void examine_edge(edge_descriptor v, modules_tree_type const& g);

  boost::shared_ptr<shared_state> state;
};

} }

#endif
