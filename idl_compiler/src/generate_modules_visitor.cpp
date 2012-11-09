/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_modules_visitor.hpp>
#include <morbid/idl_compiler/module.hpp>

#include <boost/range.hpp>

namespace morbid { namespace idl_compiler {

void generate_modules_visitor::examine_vertex
  (vertex_descriptor v, modules_tree_type const& modules)
{
  typedef typename boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), modules);
  module const& m = *boost::get(map, v);
  std::cout << "Module " << m.name << std::endl;
  typedef typename modules_tree_type::in_edge_iterator in_edge_iterator;

  // std::pair<in_edge_iterator, in_edge_iterator>
  //   edges = in_edges(v, modules);
  // if(boost::distance(edges))
  // {
  //   assert(boost::distance(edges) == 1);
  //   do
  //   {
  //     vertex_descriptor v = source(*edges.first, modules);
  //     module const& m = *boost::get(map, v);
  //     module_path.push_back(m.name);
  //     edges = in_edges(v, modules);
  //   }
  //   while(boost::distance(edges));
  // }

  // std::copy(module_path.rbegin(), module_path.rend()
  //           , std::ostream_iterator<std::string>(std::cout, "::"));
  // std::cout << std::endl;
}

// void generate_modules_visitor::examine_edge
//   (edge_descriptor e, modules_tree_type const& modules)
// {
//   typedef typename boost::property_map<modules_tree_type, module_property_t>
//     ::const_type module_map;
//   module_map map = get(module_property_t(), modules);
//   module const& src = *boost::get(map, source(e, modules));
//   module const& tgt = *boost::get(map, target(e, modules));
//   std::cout << "Edge " << src.name << " x "
//             << tgt.name << std::endl;
// }


} }

