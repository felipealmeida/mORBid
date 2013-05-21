/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "reference.hpp"
#include <morbid/corba.hpp>

#include <boost/thread.hpp>

#include <fstream>

struct some_interface_impl
{
  some_interface_impl(corba::orb orb)
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
      orb.stop();
    }
  }

  int foo_;
  corba::orb orb;
};

void run_orb(corba::orb orb)
{
  orb.run();
}

int main(int argc, char* argv[])
{
  corba::orb orb;;

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  some_interface_impl some_interface(orb);
  some_other_interface_ref some_other_interface (orb, ior);

  boost::thread orb_thread(boost::bind(&run_orb, orb));
  
  some_other_interface.foo1(boost::ref(some_interface));
  ::some_interface some_interface_out_ref;
  some_other_interface.foo2(some_interface_out_ref);
  some_other_interface.foo3(some_interface_out_ref);
  ::some_interface some_interface_ref = some_other_interface.foo4();

  std::cout << "All functions called" << std::endl;

  orb_thread.join();

  assert(some_interface.foo_ == 4);

  std::cout << "Finished" << std::endl;
}
