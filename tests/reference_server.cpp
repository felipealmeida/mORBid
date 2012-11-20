/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "reference.h"
#include <CORBA.h>

#include <fstream>

struct some_other_interface_impl : POA_some_other_interface
{
  some_other_interface_impl(CORBA::ORB_var orb)
    : p(some_interface::_nil()), foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false)
    , orb(orb)
  {}

  void foo1(some_interface_ptr s)
  {
    std::cout << "struct_interface_impl::foo1 called" << std::endl;
    assert(!foo1_ && !foo2_ && !foo3_ && !foo4_ && CORBA::is_nil(p));
    foo1_ = true;
    p = s;
  }

  void foo2(some_interface_ptr& s)
  {
    std::cout << "struct_interface_impl::foo2 called" << std::endl;
    assert(foo1_ && !foo2_ && !foo3_ && !foo4_);
    foo2_ = true;
    s = p;
  }

  void foo3(some_interface_ptr& s)
  {
    std::cout << "struct_interface_impl::foo3 called" << std::endl;
    assert(foo1_ && foo2_ && !foo3_ && !foo4_);
    foo3_ = true;
    s = p;
  }

  some_interface_ptr foo4()
  {
    std::cout << "struct_interface_impl::foo4 called" << std::endl;
    assert(foo1_ && foo2_ && foo3_ && !foo4_);
    foo4_ = true;
    orb->shutdown(true);
    return p;
  }

  some_interface_ptr p;
  bool foo1_, foo2_, foo3_, foo4_;
  CORBA::ORB_var orb;
};

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  
  some_other_interface_impl some_other_interface(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&some_other_interface);

  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  if(argc > 1)
  {
    std::ofstream ofs(argv[1]);
    ofs << str.in() << std::endl;
  }
  else
    std::cout << str.in() << std::endl;

  std::cout << "Running" << std::endl;

  poa_manager->activate();
  orb->run();

  assert(some_other_interface.foo1_ && some_other_interface.foo2_ && some_other_interface.foo3_ && some_other_interface.foo4_);
}
