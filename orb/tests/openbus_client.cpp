/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#define MORBID_OPENBUS
#include "access_control.hpp"
#include "scs.hpp"
#include <morbid/corba.hpp>
#include <morbid/openbus.hpp>

int main()
{
  corba::orb orb(morbid::openbus, "localhost", 2089, "tester", "tester");

  
}

