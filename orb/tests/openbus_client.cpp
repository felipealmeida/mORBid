/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "access_control.hpp"
#include "offer_registry.hpp"
#include <morbid/corba.hpp>
#include <morbid/openbus.hpp>

int main()
{
  corba::orb orb(morbid::openbus, "localhost", 2089, "tester", "tester");

  namespace offer_registry = tecgraf::openbus::core::v2_0::services::offer_registry;

  morbid::structured_ior sior = {"IDL:scs/core/IComponent:1.0"};
  const char openbus_key[] = "OpenBus_2_0";
  std::vector<char> ob(openbus_key, openbus_key + sizeof(openbus_key)-1);
  morbid::iiop::profile_body pb("localhost", 2089, ob);
  sior.structured_profiles.push_back(pb);

  ::scs::core::IComponent_ref openbus_component_ref(orb, sior);
  morbid::object obj = openbus_component_ref.getFacet(offer_registry::OfferRegistry_concept::type_id());
  offer_registry::OfferRegistry registry = morbid::reference_cast<offer_registry::OfferRegistry>(obj, orb);
  
  std::vector<offer_registry::ServiceProperty> properties(1);
  properties[0].name = "offer.domain";
  properties[0].value = "Demos";

  std::vector<offer_registry::ServiceOfferDesc> offers
    = registry.findServices(properties);

  std::cout << "Found " << offers.size() << "offers" << std::endl;
}

