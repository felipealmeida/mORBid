/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generator/stub_generator.hpp>
#include <morbid/idl_compiler/generator/local_stub_generator.hpp>
#include <morbid/idl_compiler/generator/poa_stub_generator.hpp>
#include <morbid/idl_compiler/generator/remote_stub_generator.hpp>
#include <morbid/idl_compiler/generator/typedef_generator.hpp>
#include <morbid/idl_compiler/generate_cpp_modules_visitor.hpp>
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

void generate_cpp_modules_visitor::examine_vertex
  (vertex_descriptor v, modules_tree_type const& modules)
{
  namespace karma = boost::spirit::karma;
  namespace phoenix = boost::phoenix;
  typedef boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), modules);
  module const& m = *boost::get(map, v);
  std::cout << "Module " << m.name << std::endl;
  typedef modules_tree_type::in_edge_iterator in_edge_iterator;

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

  morbid::idl_compiler::generator::header_remote_stub_generator
    <output_iterator_type, parser_iterator_type>
    header_remote_stub_generator;
  morbid::idl_compiler::generator::cpp_stub_generator
    <output_iterator_type, parser_iterator_type>
    cpp_stub_generator;

  std::vector<std::string> modules_name;
  for(std::vector<vertex_descriptor>::const_iterator
        first = state->opened_modules.begin()
        , last = state->opened_modules.end()
        ;first != last; ++first)
  {
    module const* mx = &*boost::get(map, *first);
    modules_name.push_back(mx->name);
  }

  if (!m.interfaces.empty())
  {
    
    open_namespace(state->iterator, ""); // anonymous namespace

    for(std::vector<interface_>::const_iterator first = m.interfaces.begin()
          , last = m.interfaces.end(); first != last; ++first)
    {
      std::vector<std::string> base_name(modules_name);
      base_name.push_back(first->definition.name);
      bool r = karma::generate(state->iterator, header_remote_stub_generator
                               (phoenix::val(*first), phoenix::ref(base_name))
                               , first->definition);
      if(!r) throw std::runtime_error("Failed generating header_remote_stub_generator");
    }

    *state->iterator++ = '}';
    karma::generate(state->iterator, karma::eol);
  }

  for(std::vector<interface_>::const_iterator first = m.interfaces.begin()
        , last = m.interfaces.end(); first != last; ++first)
  {
    std::vector<std::string> base_name(boost::next(modules_name.begin()), modules_name.end());
    base_name.push_back(first->definition.name);
    bool r = karma::generate(state->iterator, cpp_stub_generator(phoenix::val(*first), phoenix::ref(base_name))
                             , first->definition);
    if(!r) throw std::runtime_error("Failed generating header_stub_generator");
  }

}

void generate_cpp_poa_modules_visitor::examine_vertex
  (vertex_descriptor v, modules_tree_type const& modules)
{
  namespace karma = boost::spirit::karma;
  namespace phoenix = boost::phoenix;
  typedef boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), modules);
  module const& m = *boost::get(map, v);
  std::cout << "Module " << m.name << std::endl;
  typedef modules_tree_type::in_edge_iterator in_edge_iterator;

  if(!m.interfaces.empty())
  {

  std::vector<std::string> modules_names;
  {
    vertex_descriptor vx = v;
    module const* mx = &*boost::get(map, vx);
    std::pair<in_edge_iterator, in_edge_iterator> edges;
    while(mx->name != "")
    {
      modules_names.push_back(mx->name);

      edges = in_edges(vx, modules);
      vx = source(*edges.first, modules);
      mx = &*boost::get(map, vx);
    }
  }

  bool prepend_poa = modules_names.empty();
  
  if(!prepend_poa)
  {
    std::string name = "POA_" + modules_names.back();
    open_namespace(state->iterator, name);
  }

  for(std::vector<std::string>::const_reverse_iterator
        first = prepend_poa ? modules_names.rbegin() : boost::next(modules_names.rbegin())
        , last = modules_names.rend()
        ;first != last; ++first)
    open_namespace(state->iterator, *first);

  morbid::idl_compiler::generator::cpp_poa_stub_generator
    <output_iterator_type, parser_iterator_type>
    cpp_poa_stub_generator;
  morbid::idl_compiler::generator::header_local_stub_generator
    <output_iterator_type, parser_iterator_type>
    header_local_stub_generator;

  if(!m.interfaces.empty())
  {
    open_namespace(state->iterator, ""); // anonymous namespace

    for(std::vector<interface_>::const_iterator first = m.interfaces.begin()
          , last = m.interfaces.end(); first != last; ++first)
    {
      std::vector<std::string> base_name(modules_names.rbegin(), modules_names.rend());
      bool r = karma::generate(state->iterator, header_local_stub_generator
                               (phoenix::val(*first)
                                , phoenix::val(base_name))
                               , first->definition);
      if(!r) throw std::runtime_error("Failed generating header_local_stub_generator");
    }

    *state->iterator++ = '}';
    karma::generate(state->iterator, karma::eol);
  }

  for(std::vector<interface_>::const_iterator first = m.interfaces.begin()
        , last = m.interfaces.end(); first != last; ++first)
  {
    bool r = karma::generate(state->iterator, cpp_poa_stub_generator(phoenix::val(*first)
                                                                     , prepend_poa)
                             , first->definition);
    if(!r) throw std::runtime_error("Failed generating header_poa_stub_generator");
  }

  for(std::vector<std::string>::const_reverse_iterator
        first = modules_names.rbegin()
        , last = modules_names.rend()
        ;first != last; ++first)
  {
    *state->iterator++ = '}';
    *state->iterator++ = ' ';
    *state->iterator++ = '/';
    *state->iterator++ = '/';
    state->iterator = std::copy(first->begin(), first->end(), state->iterator);
    karma::generate(state->iterator, karma::eol);
  }
  }
}

} }

