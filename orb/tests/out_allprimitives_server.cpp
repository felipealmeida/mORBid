/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "out_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct out_allprimitives_impl
{
  out_allprimitives_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false)
    , orb(orb) {}

  void foo1(bool& b)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    b = true;
    foo1_ = true;
  }
  void foo2(char& c)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    c = 'c';
    foo2_ = true;
  }
  void foo3(double& d)
  {
    std::cout << "== foo3" << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    d = 2.0;
    foo3_ = true;
  }
  void foo4(float& f)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    f = 2.0f;
    foo4_ = true;
  }
  void foo5(morbid::long_& l)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    l = 2l;
    foo5_ = true;
  }
  void foo6(morbid::octet& s)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    s = 'c';
    foo6_ = true;
  }
  void foo7(morbid::short_& s)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    s = 2;
    foo7_ = true;
  }
  void foo8(std::string& str)
  {
    std::cout << "== foo8 string: " << str << std::endl;
    assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    str = "abc";
    foo8_ = true;
    orb.stop();
  }

  bool foo1_, foo2_, foo3_, foo4_, foo5_
       , foo6_, foo7_, foo8_, foo9_;
  corba::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  out_allprimitives_impl out_allprimitives(orb);
  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< out_allprimitives_concept>
       (out_allprimitives), output);
  }
  
  std::cout << "Running" << std::endl;

  orb.run();

  assert(out_allprimitives.foo7_ && out_allprimitives.foo6_ && out_allprimitives.foo5_
         && out_allprimitives.foo4_ && out_allprimitives.foo3_ && out_allprimitives.foo2_
         && out_allprimitives.foo1_);
}
