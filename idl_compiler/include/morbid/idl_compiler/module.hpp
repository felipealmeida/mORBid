/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_MODULE_HPP
#define MORBID_IDL_COMPILER_MODULE_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>

#include <boost/graph/adjacency_list.hpp>

namespace morbid { namespace idl_compiler {

struct typedef_
{
  typedef_def_type definition;
  lookuped_type lookup;

  typedef_(typedef_def_type definition)
    : definition(definition) {}
};

struct find_typedef_by_name
{
  find_typedef_by_name(std::string const& name)
    : name(name) {}

  bool operator()(typedef_ const& i) const
  {
    return i.definition.name == name;
  }

  std::string name;
};


struct exception
{
};

struct struct_
{
  struct_def_type definition;
  struct_(struct_def_type definition)
    : definition(definition) {}
  typedef std::map<type_spec, lookuped_type> lookups_type;
  lookups_type lookups;
};

inline std::ostream& operator<<(std::ostream& os, struct_ const& s)
{
  return os << "[struct_ definition: " << s.definition << "]";
}

struct find_struct_by_name
{
  find_struct_by_name(std::string const& name)
    : name(name) {}

  bool operator()(struct_ const& i) const
  {
    return i.definition.name == name;
  }

  std::string name;
};

struct module
{
  module() {}
  module(std::string name)
    : name(name) {}

  std::string name;
  std::vector<idl_compiler::interface_> interfaces;
  std::vector<idl_compiler::typedef_> typedefs;
  std::vector<idl_compiler::exception> exceptions;
  std::vector<idl_compiler::struct_> structs;
};

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::struct_
                           , ( ::morbid::idl_compiler::struct_def_type, definition)
                           (::morbid::idl_compiler::struct_::lookups_type, lookups));

#endif
