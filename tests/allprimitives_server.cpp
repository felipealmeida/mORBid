/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allprimitives.h"

struct allprimitives_impl : POA_allprimitives
{
  void foo1(bool)
  {
    std::cout << "foo1" << std::endl;
  }
  void foo2(char)
  {
    std::cout << "foo2" << std::endl;
  }
  void foo3(double)
  {
    std::cout << "foo3" << std::endl;
  }
  void foo4(float)
  {
    std::cout << "foo4" << std::endl;
  }
  void foo5(long)
  {
    std::cout << "foo5" << std::endl;
  }
  void foo6(const unsigned char*)
  {
    std::cout << "foo6" << std::endl;
  }
  void foo7(short)
  {
    std::cout << "foo7" << std::endl;
  }
  void foo8(const char*)
  {
    std::cout << "foo8" << std::endl;
  }
  void foo9(wchar_t)
  {
    std::cout << "foo9" << std::endl;
  }
  void foo10(const wchar_t*)
  {
    std::cout << "foo10" << std::endl;
  }
  void foo11(CORBA::Any_ptr)
  {
    std::cout << "foo11" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  
  allprimitives_impl allprimitives;

  PortableServer::ObjectId_var oid = poa->activate_object (&allprimitives);

  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  std::cout << str.in() << std::endl;

  std::cout << "Running" << std::endl;

  poa_manager->activate();
  orb->run();
}
