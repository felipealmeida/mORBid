/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "return_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

struct return_allprimitives_impl
{
  return_allprimitives_impl(corba::orb orb)
    : foo1_(false), foo2_(false), foo3_(false)
    , foo4_(false), foo5_(false), foo6_(false)
    , foo7_(false), foo8_(false)
    , orb(orb) {}

  bool foo1()
  {
    std::cout << "== foo1" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(!foo5_);
    assert(!foo4_);
    assert(!foo3_);
    assert(!foo2_);
    assert(!foo1_);
    foo1_ = true;
    return true;
  }
  char foo2()
  {
    std::cout << "== foo2" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(!foo5_);
    assert(!foo4_);
    assert(!foo3_);
    assert(!foo2_);
    assert(foo1_);
    foo2_ = true;
    return 'c';
  }
  double foo3()
  {
    std::cout << "== foo3" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(!foo5_);
    assert(!foo4_);
    assert(!foo3_);
    assert(foo2_);
    assert(foo1_);
    foo3_ = true;
    return 2.0;
  }
  float foo4()
  {
    std::cout << "== foo4" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(!foo5_);
    assert(!foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo4_ = true;
    return 2.0f;
  }
  morbid::long_ foo5()
  {
    std::cout << "== foo5" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(!foo5_);
    assert(foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo5_ = true;
    return 2l;
  }
  morbid::octet foo6()
  {
    std::cout << "== foo6" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(!foo6_);
    assert(foo5_);
    assert(foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo6_ = true;
    return 'c';
  }
  morbid::short_ foo7()
  {
    std::cout << "== foo7" << std::endl;
    assert(!foo8_);
    assert(!foo7_);
    assert(foo6_);
    assert(foo5_);
    assert(foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo7_ = true;
    return 2;
  }
  std::string foo8()
  {
    std::cout << "== foo8" << std::endl;
    assert(!foo8_);
    assert(foo7_);
    assert(foo6_);
    assert(foo5_);
    assert(foo4_);
    assert(foo3_);
    assert(foo2_);
    assert(foo1_);
    foo8_ = true;
    orb.stop();
    return "qwe";
  }

  bool foo1_, foo2_, foo3_, foo4_, foo5_
     , foo6_, foo7_, foo8_;
  morbid::orb orb;
};

int main(int argc, char* argv[])
{
  corba::orb orb;

  return_allprimitives_impl return_allprimitives(orb);
  {
    std::ostream_iterator<char> output(std::cout);
    std::ofstream ofs;
    if(argc > 1)
    {
      ofs.open(argv[1]);
      output = std::ostream_iterator<char> (ofs);
    }
    corba::stringify_object_id
      (orb, orb.serve_ref< return_allprimitives_concept>
       (return_allprimitives), output);
  }

  std::cout << "Running" << std::endl;

  orb.run();

  assert(return_allprimitives.foo8_);
  assert(return_allprimitives.foo7_);
  assert(return_allprimitives.foo6_);
  assert(return_allprimitives.foo5_);
  assert(return_allprimitives.foo4_);
  assert(return_allprimitives.foo3_);
  assert(return_allprimitives.foo2_);
  assert(return_allprimitives.foo1_);
}
