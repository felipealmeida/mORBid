/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "reference.h"
#include <CORBA.h>

#include <boost/thread.hpp>

#include <fstream>

struct some_interface_impl : POA_some_interface
{
  some_interface_impl(CORBA::ORB_var orb)
    : foo_(0)
    , orb(orb)
  {}

  void foo()
  {
    std::cout << "some_interface_impl::foo()" << std::endl;
    foo_++;
    if(foo_ == 4)
    {
      std::cout << "foo called 4 times. shutdown orb" << std::endl;
      orb->shutdown(true);
    }
  }

  int foo_;
  CORBA::ORB_var orb;
};

void run_orb(CORBA::ORB_var orb)
{
  orb->run();
}

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  CORBA::Object_var poa_obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poa_obj);
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();

  some_interface_impl some_interface(orb);
  PortableServer::ObjectId_var oid = poa->activate_object (&some_interface);
  CORBA::Object_var some_interface_obj_ref = poa->id_to_reference (oid.in());
  some_interface_ptr some_interface_ref = some_interface::_narrow(some_interface_obj_ref);

  CORBA::Object_var obj = orb->string_to_object (ior.c_str());
  some_other_interface_var some_other_interface_ = some_other_interface::_narrow (obj);

  poa_manager->activate();

  boost::thread orb_thread(boost::bind(&run_orb, orb));
  
  some_other_interface_->foo1(some_interface_ref);
  some_other_interface_->foo2(some_interface_ref);
  some_other_interface_->foo3(some_interface_ref);
  some_interface_ref = some_other_interface_->foo4();

  std::cout << "All functions called" << std::endl;

  orb_thread.join();

  assert(some_interface.foo_ == 4);

  std::cout << "Finished" << std::endl;
}
