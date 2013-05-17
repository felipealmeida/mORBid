/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef USING_TAO
#include "simplest.h"
#include <CORBA.h>
#else
#include "simplestS.h"
#include <tao/corba.h>
#endif

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

#include "test_iterations.hpp"

struct simplest_impl : POA_simplest
{
  simplest_impl(CORBA::ORB_var orb)
    : i(0u), orb(orb) {}

  CORBA::Boolean foo()
  {
    if(++i == test_iterations)
      orb->shutdown(false);
    return true;
  }

  unsigned int i;
  CORBA::ORB_var orb;
};

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  
  simplest_impl simplest(orb);

  PortableServer::ObjectId_var oid = poa->activate_object (&simplest);

  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  if(argc > 1)
  {
    std::ofstream ofs(argv[1]);
    ofs << str.in() << std::endl;
  }
  else
    std::cout << str.in() << std::endl;

  std::cerr << "Running" << std::endl;

  poa_manager->activate();
  orb->run();
}
