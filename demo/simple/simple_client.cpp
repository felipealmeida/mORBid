/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <CORBA.h>

#include "file.h"

int main(int argc, char** argv)
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  assert(argc > 1);

  CORBA::Object_var obj = orb->string_to_object (argv[1]);
  HelloWorld_var hello = HelloWorld::_narrow (obj);

  if (CORBA::is_nil (hello))
  {
    std::cout << "oops: could not locate HelloWorld server" << std::endl;
    return -1;
  }

  hello->hello();
}
