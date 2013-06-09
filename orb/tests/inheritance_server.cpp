/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "inheritance.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct derived_impl
{
  derived_impl(corba::orb orb)
    : foo_(0u), bar_(0u)
    , orb(orb)
  {}

  void foo()
  {
    std::cout << "derived_impl::foo called" << std::endl;
    assert(
           (foo_ == 0 && bar_ == 1)
           || (foo_ == 1 && bar_ == 3)
           );
    foo_++;
    if(foo_ == 2u)
      orb.stop();
  }

  void bar()
  {
    std::cout << "derived_impl::bar called" << std::endl;
    assert(
           (foo_ == 0 && bar_ == 0)
           || (foo_ == 1 && bar_ == 1)
           || (foo_ == 1 && bar_ == 2)
          );
    bar_++;
  }

  std::size_t foo_, bar_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  derived_impl derived(orb);

  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< derived_concept>
       (derived), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(derived.foo_ == 2);
  assert(derived.bar_ == 3);
}
