/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "struct.h"
#include <CORBA.h>

#include <fstream>

struct struct_interface_impl : POA_struct_interface
{
  struct_interface_impl(CORBA::ORB_var orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false)
    , orb(orb)
  {}

  void foo1(some_struct const& s)
  {
    std::cout << "struct_interface_impl::foo1 called" << std::endl;
    assert(!foo1_ && !foo2_ && !foo3_ && !foo4_);
    assert(s.a1);
    assert(!s.b1);
    assert(s.c1);
    assert(s.a2 == 'a');
    assert(s.b2 == 'b');
    assert(s.a3 == 2.0);
    assert(s.a4 == 2.0f);
    assert(s.a5 == 2);
    assert(s.a6 == 'c');
    assert(s.a7 == 2);
    foo1_ = true;
  }

  void foo2(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo2 called" << std::endl;
    assert(foo1_ && !foo2_ && !foo3_ && !foo4_);
    s.a1 = false;
    s.b1 = true;
    s.c1 = false;
    s.a2 = 'c';
    s.b2 = 'd';
    s.a3 = 3.0;
    s.a4 = 3.0f;
    s.a5 = 3;
    s.a6 = 'e';
    s.a7 = 3;
    foo2_ = true;
  }

  void foo3(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo3 called" << std::endl;
    assert(foo1_ && foo2_ && !foo3_ && !foo4_);
    assert(s.a1);
    assert(!s.b1);
    assert(s.c1);
    assert(s.a2 == 'a');
    assert(s.b2 == 'b');
    assert(s.a3 == 2.0);
    assert(s.a4 == 2.0f);
    assert(s.a5 == 2);
    assert(s.a6 == 'c');
    assert(s.a7 == 2);

    s.a1 = false;
    s.b1 = true;
    s.c1 = false;
    s.a2 = 'c';
    s.b2 = 'd';
    s.a3 = 3.0;
    s.a4 = 3.0f;
    s.a5 = 3;
    s.a6 = 'e';
    s.a7 = 3;

    foo3_ = true;
  }

  some_struct foo4()
  {
    std::cout << "struct_interface_impl::foo4 called" << std::endl;
    assert(foo1_ && foo2_ && foo3_ && !foo4_);
    foo4_ = true;
    orb->shutdown(false);
    some_struct r = {true, false, true, 'a', 'b', 2.0, 2.0f, 2, 'c', 2};
    return r;
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
