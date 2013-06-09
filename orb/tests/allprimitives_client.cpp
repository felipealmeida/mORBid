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
  allprimitives.foo1(true, 'c');
  allprimitives.foo2('c', 2.0);
  allprimitives.foo3(2.0, 2.0f);
  allprimitives.foo4(2.0f, 2l);
  unsigned char asd = 'a';
  allprimitives.foo5(2l, asd);
  allprimitives.foo6(asd, 2);
  allprimitives.foo7(2, "qwe");
  allprimitives.foo8("qwe", true);
  allprimitives.foo9();
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
