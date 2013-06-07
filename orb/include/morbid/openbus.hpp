/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_OPENBUS_HPP
#define MORBID_OPENBUS_HPP

#include <morbid/orb.hpp>
#include <morbid/object.hpp>

namespace morbid {

orb::orb(openbus_type, std::string const& host, unsigned short port
         , std::string const& username, std::string const& password)
  : impl(new orb_impl)
{
  structured_ior sior = {"IDL:scs/core/IComponent:1.0"};
  const char openbus_key[] = "OpenBus_2_0";
  std::vector<char> ob(openbus_key
                       , openbus_key + sizeof(openbus_key)-1);
  iiop::profile_body pb(host, port, ob);
  sior.structured_profiles.push_back(pb);
  ::scs::core::IComponent_ref openbus_component_ref(*this, sior);
  object obj = openbus_component_ref.getFacet("IDL:tecgraf/openbus/core/v2_0/services/access_control/AccessControl:1.0");

  
}

}

#endif
