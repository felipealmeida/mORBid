/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

void foo( ::allprimitives allprimitives)
{
  allprimitives.foo1(true);
  allprimitives.foo2('c');
  allprimitives.foo3(2.0);
  allprimitives.foo4(2.0f);
  allprimitives.foo5(2l);
  unsigned char asd = 'a';
  allprimitives.foo6(asd);
  allprimitives.foo7(2);
  allprimitives.foo8("qwe");
}

int main(int argc, char* argv[])
{
  corba::orb orb;

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  allprimitives_ref allprimitives (orb, ior);
  foo(allprimitives);

  std::cout << "Finished" << std::endl;
}
