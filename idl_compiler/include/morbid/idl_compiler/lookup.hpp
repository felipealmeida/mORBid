/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_LOOKUP_HPP
#define MORBID_IDL_COMPILER_LOOKUP_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_compiler/errors.hpp>

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
    std::cout << "Primitive type lookuped " << typeid(T).name() << std::endl;
    return lookuped_type(current_module[0], modules, is_primitive_kind()); // primitive
  }
  lookuped_type operator()(idl_parser::types::sequence const& s) const
  {
    std::cout << "Looking up sequence" << std::endl;
    lookuped_type t(current_module[0], modules, is_primitive_kind());
    std::cout << "Looking up inside element" << std::endl;
    t.inside_type = boost::apply_visitor(*this, s.type.type).outside_type;
    return t;
  }
  lookuped_type operator()(idl_parser::types::scoped_name const& s) const
  {
    typedef boost::property_map<modules_tree_type, module_property_t>
      ::const_type module_map;

    assert(s.identifiers.size() >= 1);

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

      typedef std::vector<vertex_descriptor>::const_reverse_iterator module_iterator;
      module_iterator first = current_module.rbegin()
        , last = boost::prior(current_module.rend());
      do
      {
        module_map map = get(module_property_t(), modules);

        idl_compiler::module const& module = *boost::get(map, *first);
        std::cout << "Current module " << module.name << std::endl;

        std::vector<idl_parser::wave_string>::const_iterator
          id_first = s.identifiers.begin()
          , id_last = boost::prior(s.identifiers.end());

        vertex_descriptor v_lookup = *first;
        bool found = true;
        while(id_first != id_last && found)
        {
          typedef typename modules_tree_type::out_edge_iterator out_edge_iterator;
          std::pair<out_edge_iterator, out_edge_iterator> child_modules
            = out_edges(v_lookup, modules);

          found = false;
          for(;child_modules.first != child_modules.second
                ;++child_modules.first)
          {
            vertex_descriptor v = target(*child_modules.first, modules);
            idl_compiler::module const& module = *boost::get(map, v);
            
            if(module.name == *id_first)
            {
              std::cout << "Found correct module " << module.name << std::endl;
              ++id_first;
              found = true;
              v_lookup = v;
            }
          }
        }

        if(found)
        {
          idl_compiler::module const& module = *boost::get(map, v_lookup);
          std::cout << "Found correct module " << module.name << std::endl;

          boost::optional<kind_variant> kind = search_type(module, s.identifiers.back());
          if(kind)
          {
            assert(v_lookup != current_module[0]);
            return lookuped_type(v_lookup, modules, *kind);
          }
        }

        ++first;
      }
      while(first != last);
    }
    throw lookup_error(s);
  }
};

inline lookuped_type lookup_type_spec
  (idl_parser::type_spec t, std::vector<vertex_descriptor>const& current_module
   , modules_tree_type const& modules)
{
  return boost::apply_visitor
    (lookup_type_spec_functional(current_module, modules), t.type);
}

} }

#endif
