/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP
#define MORBID_IDL_COMPILER_LOOKUPED_TYPE_HPP

namespace morbid { namespace idl_compiler {

struct lookuped_type
{
  vertex_descriptor outside_type;
  boost::optional<vertex_descriptor> inside_type; // template (sequences, etc)

  lookuped_type(vertex_descriptor outside_type)
    : outside_type(outside_type) {}
  lookuped_type(vertex_descriptor outside_type, vertex_descriptor inside_type)
    : outside_type(outside_type), inside_type(inside_type) {}
};

} }

#endif
