/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef USING_TAO
#include "simplest.h"
#include <CORBA.h>
#else
#include "simplestC.h"
#include <tao/corba.h>
#endif

#include "test_iterations.hpp"

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>

#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "");

  std::string ior;
  if(argc > 1)
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }
  else
    std::getline(std::cin, ior);

  assert(!ior.empty());

  CORBA::Object_var obj = orb->string_to_object (ior.c_str());
  simplest_var simplest_ = simplest::_narrow (obj);
  
  assert(!CORBA::is_nil(simplest_));

  typedef boost::chrono::high_resolution_clock::time_point time_point;
  time_point before = boost::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i != test_iterations; ++i)
  {
    simplest_->foo();
  }

  std::cout << "Time to execute test " << boost::chrono::duration_cast<boost::chrono::seconds>
    (boost::chrono::high_resolution_clock::now() - before) << std::endl;
}
