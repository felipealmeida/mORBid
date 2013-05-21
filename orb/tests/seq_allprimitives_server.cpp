/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "seq_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct seq_allprimitives_impl
{
  seq_allprimitives_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false)
    , orb(orb) {}

  void foo1(std::vector<bool> b)
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo1_);
    assert(b.size() == 1);
    assert(b[0] == true);
    foo1_ = true;
  }
  void foo2(std::vector<char> c)
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo2_ && foo1_);
    assert(c.size() == 1);
    assert(c[0] == 'c');
    foo2_ = true;
  }
  void foo3(std::vector<double> d)
  {
    std::cout << "== foo3 " << std::endl;
    assert(!foo3_ && foo2_ && foo1_);
    assert(d.size() == 1);
    assert(d[0] == 2.0);
    foo3_ = true;
  }
  void foo4(std::vector<float> f)
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo4_ && foo3_ && foo2_ && foo1_);
    assert(f.size() == 1);
    assert(f[0] == 2.0f);
    foo4_ = true;
  }
  void foo5(std::vector<morbid::long_> l)
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(l.size() == 1);
    assert(l[0] == 2l);
    foo5_ = true;
  }
  void foo6(std::vector<morbid::octet> s)
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s.size() == 1);
    assert(s[0] == 'a');
    foo6_ = true;
  }
  void foo7(std::vector<short int> s)
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(s.size() == 1);
    assert(s[0] == 2);
    foo7_ = true;
  }
  void foo8(std::vector<std::string> const& str)
  {
    std::cout << "== foo8" << std::endl;
    assert(!foo8_ && foo7_ && foo6_ && foo5_ && foo4_ && foo3_ && foo2_ && foo1_);
    assert(str.size() == 1);
    assert(str[0] == "qwe");
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

  seq_allprimitives_impl sap(orb);
  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id(orb, orb.serve_ref< ::seq_allprimitives_concept> (sap), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(sap.foo8_ && sap.foo7_ && sap.foo6_
         && sap.foo5_ && sap.foo4_ && sap.foo3_
         && sap.foo2_ && sap.foo1_);
}
