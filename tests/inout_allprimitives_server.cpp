/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "inout_allprimitives.h"

#include <fstream>

struct inout_allprimitives_impl : POA_inout_allprimitives
{
  inout_allprimitives_impl(CORBA::ORB_var orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false), foo9_(false)
    , foo10_(false), foo11_(false)
    , orb(orb) {}

  void foo1(CORBA::Boolean_out b)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    assert(b == false);
    b = true;
    foo1_ = true;
  }
  void foo2(CORBA::Char_out c)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    assert(c == 'a');
    c = 'c';
    foo2_ = true;
  }
  void foo3(CORBA::Double_out d)
  {
    std::cout << "== foo3" << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    assert(d == 1.0);
    d = 2.0;
    foo3_ = true;
  }
  void foo4(CORBA::Float_out f)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    assert(f == 1.0f);
    f = 2.0f;
    foo4_ = true;
  }
  void foo5(CORBA::Long_out l)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(l == 1l);
    l = 2l;
    foo5_ = true;
  }
  void foo6(CORBA::Octet_out s)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 'a');
    s = 'c';
    foo6_ = true;
  }
  void foo7(CORBA::Short_out s)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 1);
    s = 2;
    foo7_ = true;
    orb->shutdown(true);
  }
  // void foo8(CORBA::String_out str)
  // {
  //   std::cout << "== foo8" << std::endl;
  //   assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
  //   assert(!std::strcmp(str, "qwe"));
  //   foo8_ = true;
  //   str = "abc";
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
  
  inout_allprimitives_impl inout_allprimitives(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&inout_allprimitives);

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

  assert(inout_allprimitives.foo7_ && inout_allprimitives.foo6_ && inout_allprimitives.foo5_
         && inout_allprimitives.foo4_ && inout_allprimitives.foo3_ && inout_allprimitives.foo2_
         && inout_allprimitives.foo1_);
}
