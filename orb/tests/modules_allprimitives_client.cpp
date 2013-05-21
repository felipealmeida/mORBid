/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "modules_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

void foo(testing::inner::modules_allprimitives i)
{
  bool b = i.foo1();
  assert(b == true);
  char c = i.foo2();
  assert(c == 'c');
  double d = i.foo3();
  assert(d == 2.0);
  float f = i.foo4();
  assert(f == 2.0f);
  morbid::long_ l = i.foo5();
  assert(l == 2);
  morbid::octet octet = i.foo6();
  assert(octet == 'c');
  morbid::short_ s = i.foo7();
  assert(s == 2);
  std::string str = i.foo8();
  assert(str == "abc");
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

  testing::inner::modules_allprimitives_ref modules_allprimitives(orb, ior);
  foo(modules_allprimitives);

  std::cout << "Finished" << std::endl;
}
