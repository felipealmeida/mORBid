/* (c) Copyright 2012 Felipe Magno de Almeida
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
    : state(new shared_state(iterator))
  {
  }

  struct shared_state
  {
    shared_state(output_iterator_type& iterator)
      : iterator(iterator) {}
    output_iterator_type& iterator;
    std::vector<vertex_descriptor> opened_modules;
  };

  void discover_vertex(vertex_descriptor v, modules_tree_type const& g);
  // void examine_edge(edge_descriptor v, modules_tree_type const& g);

  boost::shared_ptr<shared_state> state;
};

// struct generate_header_poa_modules_visitor : boost::default_bfs_visitor
// {
//   generate_header_poa_modules_visitor(output_iterator_type& iterator)
//     : state(new shared_state(iterator))
//   {
//   }

//   struct shared_state
//   {
//     shared_state(output_iterator_type& iterator)
//       : iterator(iterator) {}
//     output_iterator_type& iterator;
//   };

//   void examine_vertex(vertex_descriptor v, modules_tree_type const& g);

//   boost::shared_ptr<shared_state> state;
// };

} }

#endif
