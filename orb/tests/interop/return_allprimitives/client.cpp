/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef USING_TAO
#include "return_allprimitives.h"
#include <CORBA.h>
#else
#include "return_allprimitivesC.h"
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
  return_allprimitives_var return_allprimitives_ = return_allprimitives::_narrow (obj);
  
  assert(!CORBA::is_nil(return_allprimitives_));
  CORBA::Boolean b = return_allprimitives_->foo1();
  assert(b == true);
  CORBA::Char c = return_allprimitives_->foo2();
  assert(c == 'c');
  CORBA::Double d = return_allprimitives_->foo3();
  assert(d == 2.0);
  CORBA::Float f = return_allprimitives_->foo4();
  assert(f == 2.0f);
  CORBA::Long l = return_allprimitives_->foo5();
  assert(l == 2);
  CORBA::Octet octet = return_allprimitives_->foo6();
  assert(octet == 'c');
  CORBA::Short s = return_allprimitives_->foo7();
  assert(s == 2);
  // CORBA::String_var str = "qwe";
  // return_allprimitives_->foo8(str);
  // assert(!std::strcmp(str, "abc"));
  // allprimitives_->foo9(L'q');
  // allprimitives_->foo10(L"qwe");
  // CORBA::Any_ptr any(new CORBA::Any);
  // allprimitives_->foo11(any);

  std::cout << "Finished" << std::endl;
}
