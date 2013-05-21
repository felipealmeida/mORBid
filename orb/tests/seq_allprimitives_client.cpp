/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "seq_allprimitives.hpp"
#include <morbid/corba.hpp>

#include <fstream>

void foo( ::seq_allprimitives ap)
{
  std::vector<bool> bs; bs.push_back(true);
  ap.foo1(bs);
  std::vector<char> cs; cs.push_back('c');
  ap.foo2(cs);
  std::vector<double> ds; ds.push_back(2.0);
  ap.foo3(ds);
  std::vector<float> fs; fs.push_back(2.0f);
  ap.foo4(fs);
  std::vector<morbid::long_> ls; ls.push_back(2l);
  ap.foo5(ls);
  std::vector<morbid::octet> ucs; ucs.push_back('a');
  ap.foo6(ucs);
  std::vector<short int> is; is.push_back(2);
  ap.foo7(is);
  std::vector<std::string> ss; ss.push_back("qwe");
  ap.foo8(ss);
}

int main(int argc, char* argv[])
{
  corba::orb orb;

  assert(argc > 1);

  std::string ior;
  {
    std::ifstream ifs(argv[1]);
    std::getline(ifs, ior);
  }

  seq_allprimitives_ref sap (orb, ior);
  foo(sap);

  std::cout << "Finished" << std::endl;
}
