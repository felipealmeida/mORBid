/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "reference.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct some_other_interface_impl
{
  some_other_interface_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false)
    , orb(orb)
  {}

  void foo1(some_interface s)
  {
    std::cout << "some_other_interface_impl::foo1 called" << std::endl;
    assert(!!s);
    assert(!p);
    assert(!foo4_);
    assert(!foo3_);
    assert(!foo2_);
    assert(!foo1_);
    foo1_ = true;
    p = s;
    assert(!!p);
    p.foo();
  }

  void foo2(some_interface& s)
  {
    std::cout << "some_other_interface_impl::foo2 called" << std::endl;
    assert(!s);
    assert(!!p);
    assert(!foo4_);
    assert(!foo3_);
    assert(!foo2_);
    assert(foo1_);
    foo2_ = true;
    s = p;
    p.foo();
  }

  void foo3(some_interface& s)
  {
    std::cout << "some_other_interface_impl::foo3 called" << std::endl;
    assert(!!s);
    assert(!!p);
    assert(!foo4_);
    assert(!foo3_);
    assert(foo2_);
    assert(foo1_);
    foo3_ = true;
    s = p;
    p.foo();
  }

  some_interface foo4()
  {
    std::cout << "some_other_interface_impl::foo4 called" << std::endl;
    assert(!!p);
    assert(!foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo4_ = true;
    p.foo();
    orb.stop();
    return p;
  }

  some_interface p;
  bool foo1_, foo2_, foo3_, foo4_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  some_other_interface_impl some_other_interface(orb);

  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< some_other_interface_concept>
       (some_other_interface), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(some_other_interface.foo1_);
  assert(some_other_interface.foo2_);
  assert(some_other_interface.foo3_);
  assert(some_other_interface.foo4_);
}
