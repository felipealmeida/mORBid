/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP
#define MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/kinds.hpp>

#include <ostream>

namespace morbid { namespace idlc {

struct lookuped_type
{
  vertex_descriptor outside_type;
  boost::optional<vertex_descriptor> inside_type; // template (sequences, etc)
  modules_tree_type const* modules;
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

inline std::ostream& operator<<(std::ostream& os, lookuped_type l)
{
  return os << "lookuped_type";
}

} }

#endif
