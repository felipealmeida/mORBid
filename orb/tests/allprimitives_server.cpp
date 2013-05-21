/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct allprimitives_impl
{
  allprimitives_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false), foo9_(false)
    , foo10_(false), foo11_(false)
    , orb(orb) {}

  void foo1(bool b)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    assert(b == true);
    foo1_ = true;
  }
  void foo2(char c)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    assert(c == 'c');
    foo2_ = true;
  }
  void foo3(double d)
  {
    std::cout << "== foo3 " << d << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    assert(d == 2.0);
    foo3_ = true;
  }
  void foo4(float f)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    assert(f == 2.0f);
    foo4_ = true;
  }
  void foo5(long l)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(l == 2l);
    foo5_ = true;
  }
  void foo6(unsigned char s)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 'a');
    foo6_ = true;
  }
  void foo7(short s)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 2);
    foo7_ = true;
  }
  void foo8(std::string const& str)
  {
    std::cout << "== foo8" << std::endl;
    assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(str == "qwe");
    foo8_ = true;
    orb.stop();
  }

  bool foo1_, foo2_, foo3_, foo4_, foo5_
       , foo6_, foo7_, foo8_, foo9_, foo10_
       , foo11_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  allprimitives_impl allprimitives(orb);
  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id(orb, orb.serve_ref< ::allprimitives_concept> (allprimitives), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(allprimitives.foo8_ && allprimitives.foo7_ && allprimitives.foo6_
         && allprimitives.foo5_ && allprimitives.foo4_ && allprimitives.foo3_
         && allprimitives.foo2_ && allprimitives.foo1_);
}
