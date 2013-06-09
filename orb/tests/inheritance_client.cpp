/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "inheritance.hpp"
#include <morbid/corba.hpp>

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

  std::cout << "Read IOR " << ior << std::endl;

  {
    derived_ref derived(orb, ior);
    {
      // does derived_ref obeys derived concepts?
      ::derived d(derived);
    }
    derived.bar();
    derived.foo();

    ::base base = derived;
    base.bar();
  }

  base_ref base(orb, ior);
  base.bar();

  ::derived derived = morbid::reference_cast< ::derived>(base, orb);

  derived.foo();

  std::cout << "Finished" << std::endl;
}
