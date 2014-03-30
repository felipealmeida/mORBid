/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "access_control.hpp"
#include "offer_registry.hpp"
#include "hello.hpp"
#include "component_scs.hpp"
#include <morbid/corba.hpp>
#include <morbid/openbus.hpp>

#include <boost/thread.hpp>

struct hello
{
  hello(morbid::orb orb)
    : orb(orb) {}

  void sayHello()
  {
    std::cout << "Hello" << std::endl;
    orb.stop();
  }
  
  morbid::orb orb;
};

void run_orb(corba::orb orb)
{
  orb.run();
}

int main()
{
  corba::orb orb(morbid::openbus, "localhost", 2089, "demo", "demo");

  namespace offer_registry = tecgraf::openbus::core::v2_0::services::offer_registry;

  morbid::structured_ior sior = {::scs::core::IComponent_concept::type_id()};
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

  boost::thread orb_thread(boost::bind(&run_orb, orb));

  registry.registerService(component<morbid::reference<hello_module::Hello_concept> >
                           (orb, "hello"
                            , morbid::reference<hello_module::Hello_concept>(hello(orb)))
                           , properties);

  orb_thread.join();
}

