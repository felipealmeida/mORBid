/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allprimitives.h"

#include <fstream>

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
  allprimitives_var allprimitives_ = allprimitives::_narrow (obj);
  
  assert(!CORBA::is_nil(allprimitives_));
  allprimitives_->foo1(true);
  allprimitives_->foo2('c');
  allprimitives_->foo3(2.0);
  allprimitives_->foo4(2.0f);
  allprimitives_->foo5(2l);
  unsigned char asd = 'a';
  allprimitives_->foo6(asd);
  allprimitives_->foo7(2);
  allprimitives_->foo8("qwe");
  // allprimitives_->foo9(L'q');
  // allprimitives_->foo10(L"qwe");
  // CORBA::Any_ptr any(new CORBA::Any);
  // allprimitives_->foo11(any);

  std::cout << "Finished" << std::endl;
}
