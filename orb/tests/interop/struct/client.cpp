/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef USING_TAO
#include "struct.h"
#include <CORBA.h>
#else
#include "structC.h"
#include <tao/corba.h>
#endif

#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  CORBA::Object_var obj = orb->string_to_object (ior.c_str());
  struct_interface_var struct_interface_ = struct_interface::_narrow (obj);
  
  some_struct s = {true, false, true, 'a', 'b', 2.0, 2.0f, 2, 'c', 2};
  assert(!CORBA::is_nil(struct_interface_));
  struct_interface_->foo1(s);
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
  
  struct_interface_->foo2(s);

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

  struct_interface_->foo3(s);

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

  s = struct_interface_->foo4();

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
