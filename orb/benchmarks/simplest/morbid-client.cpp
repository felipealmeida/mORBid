/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "simplest.hpp"
#include "test_iterations.hpp"
#include <morbid/corba.hpp>

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>

#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char* argv[])
{
  corba::orb orb;

  std::string ior;
  if(argc > 1)
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }
  else
    std::getline(std::cin, ior);

  assert(!ior.empty());

  ::simplest_ref simplest (orb, ior);

  typedef boost::chrono::high_resolution_clock::time_point time_point;
  time_point before = boost::chrono::high_resolution_clock::now();

  for(unsigned int i = 0; i != test_iterations; ++i)
  {
    simplest.foo();
  }

  std::cout << "Time to execute test " << boost::chrono::duration_cast<boost::chrono::seconds>
    (boost::chrono::high_resolution_clock::now() - before) << std::endl;
}
