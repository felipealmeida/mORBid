/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "fwd_ref.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct interface2_impl;

struct interface1_impl
{
  interface1_impl(corba::orb orb)
    : orb(orb)
  {}

  interface2_impl foo();

  corba::orb orb;
};

struct interface2_impl
{
  interface2_impl(corba::orb orb)
    : orb(orb)
  {}

  interface1_impl foo()
  {
    return interface1_impl(orb);
  }

  corba::orb orb;
};

interface2_impl interface1_impl::foo()
{
  orb.stop();
  return interface2_impl(orb);
}

int main(int argc, char* argv[])
{
  corba::orb orb;

  interface2_impl interface2(orb);

  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< interface2_concept>
       (interface2), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();
}
