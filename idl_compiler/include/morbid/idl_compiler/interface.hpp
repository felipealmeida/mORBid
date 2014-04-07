/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_INTERFACE_HPP
#define MORBID_IDL_COMPILER_INTERFACE_HPP

#include <morbid/idl_compiler/common_types.hpp>
#include <morbid/idl_compiler/lookuped_type.hpp>
#include <morbid/idl_parser/interface_def.hpp>

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <map>
#include <ostream>

namespace morbid { namespace idlc {

struct interface_
{
  idl_parser::interface_def definition;
  interface_(idl_parser::interface_def definition)
    : definition(definition) {}
  typedef std::map<idl_parser::type_spec, lookuped_type> lookups_type;
  lookups_type lookups;
};

struct find_interface_by_name
{
  find_interface_by_name(idl_parser::wave_string const& name)
    : name(name) {}

  bool operator()(interface_ const& i) const
  {
    return i.definition.name == name;
  }

  idl_parser::wave_string name;
};

inline std::ostream& operator<<(std::ostream& os, interface_ const& i)
{
  return os;
}

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::idlc::interface_
                           , ( ::morbid::idl_parser::interface_def, definition)
                           (::morbid::idlc::interface_::lookups_type, lookups));

#endif
