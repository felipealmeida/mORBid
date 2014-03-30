/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_OPENBUS_DEMO_COMPONENT_SCS
#define MORBID_OPENBUS_DEMO_COMPONENT_SCS

template <typename R>
struct component;

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

template <typename R>
struct meta_interface
{
  meta_interface(component<R>& c)
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

  component<R>* c;
};

template <typename R>
struct component
{
  component(morbid::orb orb, std::string name, R ref)
    : orb(orb), ref_name(name), ref(ref)
  {}

  void startup() {}
  void shutdown () {}
  morbid::object getFacet(std::string const& interface_)
  {
    std::cout << "component::getFacet " << interface_ << std::endl;
    // if(interface_ == hello_module::Hello_concept::type_id())
    // {
    //   std::cout << "Hello" << std::endl;
    //   return morbid::reference<hello_module::Hello_concept>
    //     (hello_module::Hello(hello(orb)));
    // }
    if(interface_ == ref._object_registration_copy(orb).type_id())
    {
      std::cout << "Component" << std::endl;
      return ref;
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
        (scs::core::IMetaInterface(meta_interface<R>(*this)));
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
  std::string ref_name;
  R ref;
};

template <typename R>
std::vector<scs::core::FacetDescription> meta_interface<R>::getFacets()
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
  // facets[3].name = "hello";
  // facets[3].interface_name = hello_module::Hello_concept::type_id();
  // facets[3].facet_ref = morbid::reference<hello_module::Hello_concept>
  //   (hello(c->orb));
  facets[3].name = c->ref_name;
  facets[3].interface_name = c->ref._object_registration_copy(c->orb).type_id();
  facets[3].facet_ref = c->ref;
  return facets;
}

#endif
