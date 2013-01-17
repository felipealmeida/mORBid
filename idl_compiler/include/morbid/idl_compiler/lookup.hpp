/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOOKUP_HPP
#define MORBID_IDL_COMPILER_LOOKUP_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>

#include <boost/optional.hpp>

namespace morbid { namespace idl_compiler {

struct lookup_type_spec_functional
{
  typedef lookuped_type result_type;
  lookup_type_spec_functional(std::vector<vertex_descriptor>const& current_module
                              , modules_tree_type const& modules)
    : current_module(current_module), modules(modules)
  {}

  std::vector<vertex_descriptor>const& current_module;
  modules_tree_type const& modules;

  template <typename T>
  lookuped_type operator()(T) const
  {
    return lookuped_type(current_module[0], modules, is_primitive_kind()); // primitive
  }
  lookuped_type operator()(idl_parser::types::scoped_name const& s) const
  {
    if(s.identifiers.size() == 1 && s.identifiers[0] == "string")
      return lookuped_type(current_module[0], modules, is_interface_kind());

    std::cout << "looking up " << s << std::endl;
    if(s.globally_qualified) // bottom top
    {
      std::cout << "bottom top" << std::endl;
    }
    else // top bottom
    {
      std::cout << "top bottom" << std::endl;

      if(s.identifiers.size() == 1)
      {
        std::cout << "Just identifier " << std::endl;
        typedef std::vector<vertex_descriptor>::const_reverse_iterator module_iterator;
        module_iterator first = current_module.rbegin()
          , last = boost::prior(current_module.rend());
        do
        {
          typedef boost::property_map<modules_tree_type, module_property_t>
            ::const_type module_map;
          module_map map = get(module_property_t(), modules);

          std::vector<idl_compiler::interface_>const&interfaces = boost::get(map, current_module.back())->interfaces;
          std::vector<idl_compiler::interface_>::const_iterator interface_iterator
            = std::find_if(interfaces.begin(), interfaces.end(), find_interface_by_name(s.identifiers[0]));
          if(interface_iterator != interfaces.end())
          {
            std::cout << "Is a interface" << std::endl;
            return lookuped_type(*first, modules, is_interface_kind());
          }
          else
          {
            std::cout << "No interface from this module" << std::endl;
          }

          std::vector<idl_compiler::typedef_>const&typedefs = boost::get(map, current_module.back())->typedefs;
          std::vector<idl_compiler::typedef_>::const_iterator typedef_iterator
            = std::find_if(typedefs.begin(), typedefs.end(), find_typedef_by_name(s.identifiers[0]));
          if(typedef_iterator != typedefs.end())
          {
            std::cout << "Is a typedef " << typedef_iterator->definition.name << std::endl;
            return lookuped_type(*first, modules, is_typedef_kind());
          }
          else
          {
            std::cout << "No typedef from this module" << std::endl;
          }

          std::vector<idl_compiler::struct_>const&structs = boost::get(map, current_module.back())->structs;
          std::vector<idl_compiler::struct_>::const_iterator struct_iterator
            = std::find_if(structs.begin(), structs.end(), find_struct_by_name(s.identifiers[0]));
          if(struct_iterator != structs.end())
          {
            std::cout << "Is a struct " << struct_iterator->definition.name << std::endl;
            return lookuped_type(*first, modules, is_struct_kind());
          }
          else
          {
            std::cout << "No typedef from this module" << std::endl;
          }

          ++first;
        }
        while(first != last);
      }
    }
    throw std::runtime_error("Not found type");
  }
  lookuped_type operator()(idl_parser::types::sequence<parser_iterator_type> const& s) const
  {
    return lookuped_type(current_module[0], modules, is_primitive_kind()); // primitive
  }
};

inline lookuped_type lookup_type_spec
  (type_spec t, std::vector<vertex_descriptor>const& current_module
   , modules_tree_type const& modules)
{
  return boost::apply_visitor
    (lookup_type_spec_functional(current_module, modules), t.type);
}

} }

#endif
