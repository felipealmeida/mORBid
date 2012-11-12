/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "return_allprimitives.h"

#include <fstream>

struct return_allprimitives_impl : POA_return_allprimitives
{
  return_allprimitives_impl(CORBA::ORB_var orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false), foo9_(false)
    , foo10_(false), foo11_(false)
    , orb(orb) {}

  CORBA::Boolean foo1()
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    foo1_ = true;
    return true;
  }
  CORBA::Char foo2()
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    foo2_ = true;
    return 'c';
  }
  CORBA::Double foo3()
  {
    std::cout << "== foo3" << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    foo3_ = true;
    return 2.0;
  }
  CORBA::Float foo4()
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    foo4_ = true;
    return 2.0f;
  }
  CORBA::Long foo5()
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    foo5_ = true;
    return 2l;
  }
  CORBA::Octet foo6()
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    foo6_ = true;
    return 'c';
  }
  CORBA::Short foo7()
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    foo7_ = true;
    orb->shutdown(true);
    return 2;
  }
  // void foo8(String_out str)
  // {
  //   std::cout << "== foo8" << std::endl;
  //   assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
  //   assert(!std::strcmp(str, "qwe"));
  //   foo8_ = true;
  //   orb->shutdown(true);
  // }
  // void foo9(wchar_t wc)
  // {
  //   std::cout << "== foo9" << std::endl;
  //   assert(!foo9_ && foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
  //   assert(wc == L'q');
  //   foo9_ = true;
  // }
  // void foo10(const wchar_t* str)
  // {
  //   std::cout << "== foo10" << std::endl;
  //   assert(!foo10_ && foo9_ && foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
  //   assert(false);
  //   foo10_ = true;
  // }
  // void foo11(CORBA::Any_ptr)
  // {
  //   std::cout << "== foo11" << std::endl;
  //   assert(!foo11_ && /*foo10_ && foo9_ &&*/ foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
  //   foo11_ = true;
  //   orb->shutdown(true);
  // }

  bool foo1_, foo2_, foo3_, foo4_, foo5_
       , foo6_, foo7_, foo8_, foo9_, foo10_
       , foo11_;
  CORBA::ORB_var orb;
};

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  
  return_allprimitives_impl return_allprimitives(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&return_allprimitives);

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

  assert(return_allprimitives.foo7_ && return_allprimitives.foo6_ && return_allprimitives.foo5_
         && return_allprimitives.foo4_ && return_allprimitives.foo3_ && return_allprimitives.foo2_
         && return_allprimitives.foo1_);
}
