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
    , orb(orb) {}

  void foo1(bool b, char c)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    assert(b == true);
    assert(c == 'c');
    foo1_ = true;
  }
  void foo2(char c, double d)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    assert(c == 'c');
    assert(d == 2.0);
    foo2_ = true;
  }
  void foo3(double d, float f)
  {
    std::cout << "== foo3 " << d << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    assert(d == 2.0);
    assert(f == 2.0f);
    foo3_ = true;
  }
  void foo4(float f, long l)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    assert(f == 2.0f);
    assert(l == 2l);
    foo4_ = true;
  }
  void foo5(long l, unsigned char s)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(l == 2l);
    assert(s == 'a');
    foo5_ = true;
  }
  void foo6(unsigned char s, short s1)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 'a');
    assert(s1 == 2);
    foo6_ = true;
  }
  void foo7(short int s, std::string str)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s == 2);
    assert(str == "qwe");
    foo7_ = true;
  }
  void foo8(std::string const& str, bool b)
  {
    std::cout << "== foo8" << std::endl;
    assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(str == "qwe");
    assert(b == true);
    foo8_ = true;
  }

  void foo9()
  {
    std::cout << "== foo9" << std::endl;
    assert(!foo9_ && foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    foo9_ = true;
    orb.stop();
  }

  bool foo1_, foo2_, foo3_, foo4_, foo5_
       , foo6_, foo7_, foo8_, foo9_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  allprimitives_impl ap(orb);
  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id(orb, orb.serve_ref< ::allprimitives_concept> (ap), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(ap.foo9_ && ap.foo8_ && ap.foo7_ && ap.foo6_
         && ap.foo5_ && ap.foo4_ && ap.foo3_
         && ap.foo2_ && ap.foo1_);
}
