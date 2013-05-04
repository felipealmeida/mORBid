/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "out_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

void foo(out_allprimitives i)
{
  bool b = false;
  i.foo1(b);
  assert(b == true);
  char c = 'a';
  i.foo2(c);
  assert(c == 'c');
  double d = 1.0;
  i.foo3(d);
  assert(d == 2.0);
  float f = 1.0f;
  i.foo4(f);
  assert(f == 2.0f);
  morbid::long_ l = 1;
  i.foo5(l);
  assert(l == 2);
  morbid::octet octet = 'a';
  i.foo6(octet);
  assert(octet == 'c');
  morbid::short_ s = 1; 
  i.foo7(s);
  assert(s == 2);
  std::string str = "qwe";
  i.foo8(str);
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

  out_allprimitives_ref out_allprimitives (orb, ior);
  foo(out_allprimitives);

  std::cout << "Finished" << std::endl;
}
