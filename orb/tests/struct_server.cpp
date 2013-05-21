/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "struct.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct struct_interface_impl
{
  struct_interface_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false), orb(orb)
  {}

  void foo1(some_struct const& s)
  {
    std::cout << "struct_interface_impl::foo1 called" << std::endl;
    assert(!foo1_ && !foo2_ && !foo3_ && !foo4_);
    assert(s.a1);
    assert(!s.b1);
    assert(s.c1);
    assert(s.a2 == 'a');
    assert(s.b2 == 'b');
    assert(s.a3 == 2.0);
    assert(s.a4 == 2.0f);
    assert(s.a5 == 2);
    assert(s.a6 == 'c');
    assert(s.a7 == 2);
    foo1_ = true;
  }

  void foo2(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo2 called" << std::endl;
    assert(foo1_ && !foo2_ && !foo3_ && !foo4_);
    s.a1 = false;
    s.b1 = true;
    s.c1 = false;
    s.a2 = 'c';
    s.b2 = 'd';
    s.a3 = 3.0;
    s.a4 = 3.0f;
    s.a5 = 3;
    s.a6 = 'e';
    s.a7 = 3;
    foo2_ = true;
  }

  void foo3(some_struct& s)
  {
    std::cout << "struct_interface_impl::foo3 called" << std::endl;
    assert(foo1_ && foo2_ && !foo3_ && !foo4_);
    assert(s.a1);
    assert(!s.b1);
    assert(s.c1);
    assert(s.a2 == 'a');
    assert(s.b2 == 'b');
    assert(s.a3 == 2.0);
    assert(s.a4 == 2.0f);
    assert(s.a5 == 2);
    assert(s.a6 == 'c');
    assert(s.a7 == 2);

    s.a1 = false;
    s.b1 = true;
    s.c1 = false;
    s.a2 = 'c';
    s.b2 = 'd';
    s.a3 = 3.0;
    s.a4 = 3.0f;
    s.a5 = 3;
    s.a6 = 'e';
    s.a7 = 3;

    foo3_ = true;
  }

  some_struct foo4()
  {
    std::cout << "struct_interface_impl::foo4 called" << std::endl;
    assert(foo1_ && foo2_ && foo3_ && !foo4_);
    foo4_ = true;
    orb.stop();
    some_struct r = {true, false, true, 'a', 'b', 2.0, 2.0f, 2, 'c', 2};
    return r;
  }

  bool foo1_, foo2_, foo3_, foo4_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  struct_interface_impl si(orb);

  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< struct_interface_concept>
       (si), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(si.foo1_ && si.foo2_ && si.foo3_ && si.foo4_);
}
