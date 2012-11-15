/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "struct.h"

#include <fstream>

struct struct_interface_impl : POA_struct_interface
{
  struct_interface_impl(CORBA::ORB_var orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false)
    , orb(orb)
  {}

  void foo1(some_struct s)
  {
    std::cout << "struct_interface_impl::foo1 called" << std::endl;
    assert(!foo1_ && !foo2_ && !foo3_ && !foo4_);
    foo1_ = true;
  }

  void foo2(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo2 called" << std::endl;
    assert(foo1_ && !foo2_ && !foo3_ && !foo4_);
    foo2_ = true;
  }

  void foo3(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo3 called" << std::endl;
    assert(foo1_ && foo2_ && !foo3_ && !foo4_);
    foo3_ = true;
  }

  some_struct foo4()
  {
    std::cout << "struct_interface_impl::foo4 called" << std::endl;
    assert(foo1_ && foo2_ && foo3_ && !foo4_);
    foo4_ = true;
    orb->shutdown(true);
    return some_struct();
  }

  bool foo1_, foo2_, foo3_, foo4_;
  CORBA::ORB_var orb;
};

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  
  struct_interface_impl struct_interface(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&struct_interface);

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

  assert(struct_interface.foo1_ && struct_interface.foo2_ && struct_interface.foo3_ && struct_interface.foo4_);
}
