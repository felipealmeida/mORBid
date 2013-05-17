/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "simplest.hpp"
#include <morbid/corba.hpp>

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

struct simplest_impl
{
  simplest_impl(corba::orb orb)
    : i(0u), orb(orb) {}

  bool foo()
  {
    // std::cout << "foo " << i+1 << std::endl;
    if(++i == 10000)
      orb.stop();
    return true;
  }

  unsigned int i;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id(orb, orb.serve_copy< ::simplest_concept> (simplest_impl(orb)), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();
}
