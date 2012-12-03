/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allprimitives.h"
#include <CORBA.h>

#include <fstream>

struct allprimitives_impl : POA_allprimitives
{
  allprimitives_impl(CORBA::ORB_var orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false), foo9_(false)
    , foo10_(false), foo11_(false)
    , orb(orb) {}

  void foo1(CORBA::Boolean b)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    assert(b == true);
    foo1_ = true;
  }
  void foo2(char c)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    assert(c == 'c');
    foo2_ = true;
  }
  void foo3(double d)
  {
    std::cout << "== foo3 " << d << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    assert(d == 2.0);
    foo3_ = true;
  }
  void foo4(float f)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    assert(f == 2.0f);
    foo4_ = true;
  }
  void foo5(CORBA::Long l)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(l == 2l);
    foo5_ = true;
  }
  void foo6(unsigned char s)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 'a');
    foo6_ = true;
  }
  void foo7(CORBA::Short s)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 2);
    foo7_ = true;
  }
  void foo8(const char* str)
  {
    std::cout << "== foo8" << std::endl;
    assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(!std::strcmp(str, "qwe"));
    foo8_ = true;
    orb->shutdown(true);
  }
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
  
  allprimitives_impl allprimitives(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&allprimitives);

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

  assert(allprimitives.foo7_ && allprimitives.foo6_ && allprimitives.foo5_
         && allprimitives.foo4_ && allprimitives.foo3_ && allprimitives.foo2_
         && allprimitives.foo1_);
}
