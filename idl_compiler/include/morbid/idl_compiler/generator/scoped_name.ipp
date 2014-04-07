/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_SCOPED_NAME_IPP
#define MORBID_IDL_COMPILER_SCOPED_NAME_IPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_compiler/generator/scoped_name.hpp>
#include <morbid/idl_compiler/generator/wave_string.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

std::vector<idl_parser::wave_string> lookuped_type_wrapper::get_type() const
{
  std::cout << "type is inside? " << inside << std::endl;
  typedef modules_tree_type::in_edge_iterator in_edge_iterator;
  typedef boost::property_map<modules_tree_type, module_property_t>
    ::const_type module_map;
  module_map map = get(module_property_t(), *l.modules);

  vertex_descriptor type_ = inside ? *l.inside_type : l.outside_type;

  std::vector<idl_parser::wave_string> module_path;
  module const& m = *boost::get(map, type_);
  module_path.push_back(m.name);

  std::pair<in_edge_iterator, in_edge_iterator>
    edges = in_edges(type_, *l.modules);
  assert(boost::distance(edges) == 0 || boost::distance(edges) == 1);

  while(boost::distance(edges))
  {
    vertex_descriptor v = source(*edges.first, *l.modules);
    module const& m = *boost::get(map, v);

    std::cout << "edge name " << m.name << std::endl;

    module_path.push_back(m.name);
    edges = in_edges(v, *l.modules);
  }

  std::copy(module_path.begin(), module_path.end()
            , std::ostream_iterator<idl_parser::wave_string>(std::cout, "::"));
  std::endl(std::cout);

  std::reverse(module_path.begin(), module_path.end());
  return module_path;
}

} } }

namespace morbid { namespace idlc { namespace generator {

template <typename OutputIterator>
scoped_name<OutputIterator>::scoped_name()
    : scoped_name::base_type(start)
{
  using karma::_1;
  using karma::_r1;
  using karma::_val;
  using phoenix::at_c;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#endif

  start =
    // I don't care for globally_qualified attribute
    // because the lookuped type _r1 is already qualified
    (wave_string % "::")[_1 = at_c<0>(_r1)]
    << "::"
    << wave_string[_1 = phoenix::back(at_c<1>(_val))]
    // << //((
    // -(is_interface_kind// /* << karma::string[_1 = "_ptr"]*/)
    //     | is_struct_kind
    //     | is_primitive_kind
    //     | is_typedef_kind
    //     )
    // variant_rule
    //   [_1 = at_c<1>(_r1)]
    ;
  is_interface_kind = karma::eps;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

} } }

#endif

