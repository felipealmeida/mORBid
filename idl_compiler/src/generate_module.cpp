/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generate_functions.hpp>

#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc {

void generate_module(
  output_iterator& output_iterator,
  morbid::idl_parser::wave_string const& module_open,
  std::vector<vertex_descriptor>& current_module,
  modules_tree_type& modules_tree,
  morbid::idlc::module_map const& map
){
  using namespace morbid::idlc;
  namespace karma = boost::spirit::karma;
  
  karma::generate(output_iterator
                  , karma::eol << karma::lit("namespace ")
                  << morbid::idlc::wave_string
                  << karma::lit(" {") << karma::eol
                  , module_open);

  typedef modules_tree_type::out_edge_iterator out_edge_iterator;
  std::pair<out_edge_iterator, out_edge_iterator> child_modules
    = out_edges(current_module.back(), modules_tree);

  bool found = false;
  vertex_descriptor v;
  for(;child_modules.first != child_modules.second
        ;++child_modules.first)
  {
    v = target(*child_modules.first, modules_tree);
    morbid::idlc::module const& module = *boost::get(map, v);
    if(module.name == module_open)
    {
      found = true;
    }
  }

  if(found)
  {
    std::cout << "Module already created, just opening" << std::endl;
    current_module.push_back(v);
  }
  else
  {
    std::cout << "Module doesn't exist yet. Creating it" << std::endl;
    current_module.push_back
      (add_vertex(module_property_type
        (boost::shared_ptr<module>(new module(module_open.c_str())))
                  , modules_tree));

    add_edge(*boost::prior(current_module.end(), 2)
              , current_module.back(), modules_tree);
  }
}

} }
