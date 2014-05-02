/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_GENERATE_FUNCTIONS_HPP
#define MORBID_IDL_COMPILER_GENERATE_FUNCTIONS_HPP

#include <morbid/idl_compiler/iterator.hpp>
#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/module_map.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/struct_def.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idlc {

void generate_interface(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::interface_def const& interface,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree);

void generate_module(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::wave_string const& module_open,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree,
  morbid::idlc::module_map const& map);

void generate_module_close(
  morbid::idlc::output_iterator& output_iterator,
  std::vector<morbid::idlc::vertex_descriptor>& current_module);

void generate_struct(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::struct_def const& struct_,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree);

void generate_typedef(
  morbid::idlc::output_iterator& output_iterator,
  morbid::idl_parser::typedef_def const& typedef_,
  std::vector<morbid::idlc::vertex_descriptor>& current_module,
  morbid::idlc::modules_tree_type& modules_tree);

} }

#endif // MORBID_IDL_COMPILER_GENERATE_FUNCTIONS_HPP
