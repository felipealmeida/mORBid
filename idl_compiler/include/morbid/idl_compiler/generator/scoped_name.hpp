/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_SCOPED_NAME_HPP
#define MORBID_IDL_COMPILER_SCOPED_NAME_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_compiler/module.hpp>
#include <morbid/idl_parser/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

struct lookuped_type_wrapper
{
  lookuped_type_wrapper() {}
  lookuped_type_wrapper(lookuped_type l)
    : l(l) {}

  std::vector<std::string> get_outside_type() const
  {
    typedef modules_tree_type::in_edge_iterator in_edge_iterator;
    typedef boost::property_map<modules_tree_type, module_property_t>
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
    std::reverse(module_path.begin(), module_path.end());
    return module_path;
  }
  typedef lookuped_type::kind_variant kind_variant;
  kind_variant kind() const
  {
    return l.kind;
  }

  lookuped_type l;
};

} } }

BOOST_FUSION_ADAPT_ADT( ::morbid::idl_compiler::generator::lookuped_type_wrapper
                        , (std::vector<std::string>, std::vector<std::string>
                           , obj.get_outside_type(), ::std::abort())
                        (::morbid::idl_compiler::generator::lookuped_type_wrapper::kind_variant
                         , ::morbid::idl_compiler::generator::lookuped_type_wrapper::kind_variant
                         , obj.kind(), ::std::abort())
                        );

namespace morbid { namespace idl_compiler { namespace generator {

template <typename OutputIterator, typename Iterator>
struct scoped_name : karma::grammar<OutputIterator, idl_parser::types::scoped_name
                                    (lookuped_type_wrapper)>
{
  scoped_name()
    : scoped_name::base_type(start)
  {
    using karma::_1;
    using karma::_r1;
    using karma::_val;
    using phoenix::at_c;

    start =
      // I don't care for globally_qualified attribute
      // because the lookuped type _r1 is already qualified
      (karma::string % "::")[_1 = at_c<0>(_r1)]
      << "::" << (karma::string % "::")
      [_1 = at_c<1>(_val)]
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
  }

  karma::rule<OutputIterator, idl_parser::types::scoped_name
              (lookuped_type_wrapper)> start;
  karma::rule<OutputIterator, idl_compiler::is_interface_kind()>
    is_interface_kind;
  karma::rule<OutputIterator, idl_compiler::is_struct_kind()>
    is_struct_kind;
  karma::rule<OutputIterator, idl_compiler::is_primitive_kind()>
    is_primitive_kind;
  karma::rule<OutputIterator, idl_compiler::is_typedef_kind()>
    is_typedef_kind;
};

} } }

#endif

