/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "struct.hpp"
#include <morbid/corba.hpp>

#include <fstream>

int main(int argc, char* argv[])
{
  corba::orb orb;

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  struct_interface_ref si (orb, ior);
  
  some_struct s = {true, false, true, 'a', 'b', 2.0, 2.0f, 2, 'c', 2};

  si.foo1(s);
  assert(s.a1);
  assert(!s.b1);
  assert(s.c1);
  assert(s.a2 == 'a');
  assert(s.b2 == 'b');
  assert(s.a3 == 2.0);
  assert(s.a4 == 2.0f);
  assert(s.a5 == 2);
  assert(s.a6 == 'c');
  assert(s.a7 == 2);
  
  si.foo2(s);

  assert(!s.a1);
  assert(s.b1);
  assert(!s.c1);
  assert(s.a2 == 'c');
  assert(s.b2 == 'd');
  assert(s.a3 == 3.0);
  assert(s.a4 == 3.0f);
  assert(s.a5 == 3);
  assert(s.a6 == 'e');
  assert(s.a7 == 3);

  some_struct s1 = {true, false, true, 'a', 'b', 2.0, 2.0f, 2, 'c', 2};
  s = s1;

  si.foo3(s);

  assert(!s.a1);
  assert(s.b1);
  assert(!s.c1);
  assert(s.a2 == 'c');
  assert(s.b2 == 'd');
  assert(s.a3 == 3.0);
  assert(s.a4 == 3.0f);
  assert(s.a5 == 3);
  assert(s.a6 == 'e');
  assert(s.a7 == 3);

  s = si.foo4();

  assert(s.a1);
  assert(!s.b1);
  assert(s.c1);
  assert(s.a2 == 'a');
  assert(s.b2 == 'b');
  assert(s.a3 == 2.0);
  assert(s.a4 == 2.0f);
  assert(s.a5 == 2);
  assert(s.a6 == 'c');
  assert(s.a7 == 2);

  std::cout << "Finished" << std::endl;
}
