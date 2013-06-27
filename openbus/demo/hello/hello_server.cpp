/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "access_control.hpp"
#include "offer_registry.hpp"
#include "hello.hpp"
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

struct receptacles
{
  scs::core::ConnectionId connect(std::string const&, morbid::object)
  {
    std::abort();
  }
  void disconnect(scs::core::ConnectionId) {}
  scs::core::ConnectionDescriptions getConnections(std::string const&)
  {
    std::abort();
  }
};

struct component;

struct meta_interface
{
  meta_interface(component& c)
    : c(&c) {}

  std::vector<scs::core::FacetDescription> getFacets();
  scs::core::FacetDescriptions getFacetsByName(scs::core::NameList names)
  {
    std::abort();
  }
  scs::core::ReceptacleDescriptions getReceptacles()
  {
    std::abort();
  }
  scs::core::ReceptacleDescriptions getReceptaclesByName(scs::core::NameList)
  {
    std::abort();
  }

  component* c;
};

struct component
{
  component(morbid::orb orb)
    : orb(orb) {}

  void startup() {}
  void shutdown () {}
  morbid::object getFacet(std::string const& interface_)
  {
    std::cout << "component::getFacet " << interface_ << std::endl;
    if(interface_ == hello_module::Hello_concept::type_id())
    {
      std::cout << "Hello" << std::endl;
      return morbid::reference<hello_module::Hello_concept>
        (hello_module::Hello(hello(orb)));
    }
    else if(interface_ == scs::core::IReceptacles_concept::type_id())
    {
      std::cout << "IReceptacles" << std::endl;
      return morbid::reference<scs::core::IReceptacles_concept>
        (scs::core::IReceptacles(receptacles()));
    }
    else if(interface_ == scs::core::IMetaInterface_concept::type_id())
    {
      std::cout << "IMetaInterface" << std::endl;
      return morbid::reference<scs::core::IMetaInterface_concept>
        (scs::core::IMetaInterface(meta_interface(*this)));
    }
    else
    {
      std::abort();
    }
  }
  morbid::object getFacetByName(std::string const& name)
  {
    if(name == "IMetaInterface")
    {
      return getFacet(scs::core::IMetaInterface_concept::type_id());
    }
    std::abort();
  }
  scs::core::ComponentId getComponentId()
  {
    std::cout << "component::getComponentId " << std::endl;
    scs::core::ComponentId id = {"Hello", 1, 0, 0, "something"};
    return id;
  }

  morbid::orb orb;
};

std::vector<scs::core::FacetDescription> meta_interface::getFacets()
{
  std::cout << "meta_interface::getFacets" << std::endl;
  std::vector<scs::core::FacetDescription> facets(4);
  facets[0].name = "IMetaInterface";
  facets[0].interface_name = ::scs::core::IMetaInterface_concept::type_id();
  facets[0].facet_ref = morbid::reference<scs::core::IMetaInterface_concept>
    (scs::core::IMetaInterface(*this));
  facets[1].name = "IReceptacles";
  facets[1].interface_name = ::scs::core::IReceptacles_concept::type_id();
  facets[1].facet_ref = morbid::reference<scs::core::IReceptacles_concept>
    (scs::core::IReceptacles(receptacles()));
  facets[2].name = "IComponent";
  facets[2].interface_name = ::scs::core::IComponent_concept::type_id();
  facets[2].facet_ref = morbid::reference<scs::core::IComponent_concept>
    (scs::core::IComponent(boost::ref(*c)));
  facets[3].name = "hello";
  facets[3].interface_name = hello_module::Hello_concept::type_id();
  facets[3].facet_ref = morbid::reference<hello_module::Hello_concept>
    (hello(c->orb));
  return facets;
}

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

  registry.registerService(component(orb), properties);

  orb_thread.join();
}

