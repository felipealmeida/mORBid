/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP
#define MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP

#include <morbid/idl_compiler/common_types.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <ostream>

namespace morbid { namespace idl_compiler {

struct is_interface_kind {};
struct is_struct_kind {};
struct is_primitive_kind {};
struct is_typedef_kind {};

struct lookuped_type
{
  vertex_descriptor outside_type;
  boost::optional<vertex_descriptor> inside_type; // template (sequences, etc)
  modules_tree_type const* modules;
  typedef boost::variant<is_interface_kind, is_struct_kind, is_primitive_kind
                         , is_typedef_kind> kind_variant;
  kind_variant kind;

  lookuped_type() : modules(0) {}
  lookuped_type(vertex_descriptor outside_type, modules_tree_type const& modules
                , kind_variant kind)
    : outside_type(outside_type), modules(&modules), kind(kind) {}
  lookuped_type(vertex_descriptor outside_type, vertex_descriptor inside_type
                , modules_tree_type const& modules
                , kind_variant kind)
    : outside_type(outside_type), inside_type(inside_type)
    , modules(&modules), kind(kind) {}
};

std::ostream& operator<<(std::ostream& os, lookuped_type l);

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_interface_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_struct_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_primitive_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_typedef_kind
                           , BOOST_PP_EMPTY())

#endif
