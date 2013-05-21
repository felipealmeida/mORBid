/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "fwd_ref.hpp"
#include <morbid/corba.hpp>

#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>

#include <fstream>

int main(int argc, char* argv[])
{
  corba::orb orb;;

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  interface2_ref i2(orb, ior);
  interface1 i1 = i2.foo();
  interface2 otheri2 = i1.foo();

  std::cout << "All functions called" << std::endl;


  std::cout << "Finished" << std::endl;
}
