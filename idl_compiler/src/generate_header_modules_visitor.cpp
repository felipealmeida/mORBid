/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generator/stub_generator.hpp>
#include <morbid/idl_compiler/generator/typedef_generator.hpp>
#include <morbid/idl_compiler/generate_header_modules_visitor.hpp>
#include <morbid/idl_compiler/module.hpp>

#include <boost/range.hpp>

namespace morbid { namespace idl_compiler {

template <typename OutputIterator>
void open_namespace(OutputIterator& iterator, std::string name)
{
  namespace karma = boost::spirit::karma;
  const char namespace_[] = "namespace ";
  iterator = std::copy(&namespace_[0], &namespace_[sizeof(namespace_)-1]
                              , iterator);
  iterator = std::copy(name.begin(), name.end(), iterator);
  *iterator++ = '{';
  karma::generate(iterator, karma::eol);
}

void generate_header_modules_visitor::examine_vertex
  (vertex_descriptor v, modules_tree_type const& modules)
{
  namespace karma = boost::spirit::karma;
  namespace phoenix = boost::phoenix;
  typedef typename boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), modules);
  module const& m = *boost::get(map, v);
  std::cout << "Module " << m.name << std::endl;
  typedef typename modules_tree_type::in_edge_iterator in_edge_iterator;

  if(state->opened_modules.size() < 2)
  {
    if(!state->opened_modules.empty())
      idl_compiler::open_namespace(state->iterator, m.name);
    state->opened_modules.push_back(v);
  }
  else
  {
    std::pair<in_edge_iterator, in_edge_iterator>
      edges = in_edges(v, modules);
    vertex_descriptor b = source(*edges.first, modules);
    assert(boost::distance(edges) == 1);
    if(state->opened_modules.back() == b)
    {
      std::cout << "Should open " << m.name << std::endl;
      idl_compiler::open_namespace(state->iterator, m.name);
      state->opened_modules.push_back(v);
    }
    else
    {
      module const& before = *boost::get(map, b);
      std::cout << "Should close " << before.name << std::endl;

      *state->iterator++ = '}';
      *state->iterator++ = ' ';
      *state->iterator++ = '/';
      *state->iterator++ = '/';
      state->iterator = std::copy(before.name.begin(), before.name.end(), state->iterator);
      karma::generate(state->iterator, karma::eol);

      state->opened_modules.pop_back();

      std::cout << "And then open " << m.name << std::endl;

      idl_compiler::open_namespace(state->iterator, m.name);
      state->opened_modules.push_back(v);
    }
  }

  morbid::idl_compiler::generator::header_stub_generator
    <output_iterator_type, parser_iterator_type>
    header_stub_generator;
  morbid::idl_compiler::generator::typedef_generator
    <output_iterator_type, parser_iterator_type>
    typedef_generator;

  for(std::vector<typedef_>::const_iterator first = m.typedefs.begin()
        , last = m.typedefs.end(); first != last; ++first)
  {
    bool r = karma::generate(state->iterator, typedef_generator(phoenix::val(first->lookup))
                             , first->definition);
    if(!r) throw std::runtime_error("Failed generating typedefs");
  }

  for(std::vector<interface_>::const_iterator first = m.interfaces.begin()
        , last = m.interfaces.end(); first != last; ++first)
  {
    bool r = karma::generate(state->iterator, header_stub_generator(phoenix::val(*first))
                             , first->definition);
    if(!r) throw std::runtime_error("Failed generating header_stub_generator");
  }

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

// void generate_header_modules_visitor::examine_edge
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

std::ostream& operator<<(std::ostream& os, lookuped_type l)
{
  typedef typename modules_tree_type::in_edge_iterator in_edge_iterator;
  os << "[lookuped_type outside_type: ";
  typedef typename boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), *l.modules);

  std::vector<std::string> module_path;
  module const& m = *boost::get(map, l.outside_type);
  module_path.push_back(m.name);

  std::pair<in_edge_iterator, in_edge_iterator>
    edges = in_edges(l.outside_type, *l.modules);
  if(boost::distance(edges))
  {
    assert(boost::distance(edges) == 1);
    do
    {
      vertex_descriptor v = source(*edges.first, *l.modules);
      module const& m = *boost::get(map, v);
      module_path.push_back(m.name);
      edges = in_edges(v, *l.modules);
    }
    while(boost::distance(edges));
  }
  std::copy(module_path.rbegin(), module_path.rend()
            , std::ostream_iterator<std::string>(os, "::"));
  return os << ']';
}

} }

