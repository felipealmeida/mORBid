/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/iiop/all.hpp>

int main()
{
  namespace giop = morbid::giop;
  namespace karma = boost::spirit::karma;
  namespace fusion = boost::fusion;
  namespace spirit = boost::spirit;
  namespace iiop = morbid::iiop;
  namespace phoenix = boost::phoenix;
  using phoenix::at_c;

  typedef fusion::vector<unsigned char, unsigned char, std::string
                         , unsigned short, std::vector<unsigned char> > attribute_type;
  typedef char* iterator_type;
  karma::rule<iterator_type, attribute_type(), spirit::locals<unsigned short> > rule;

  rule %=
  giop::compile<iiop::generator_domain>
    (
     giop::endianness
     [
      giop::octet                      // IIOP major version
      & giop::octet                    // IIOP minor version
      & giop::string                   // host
      & giop::ushort_                  // port
      & giop::sequence[giop::octet]    // object key
     ]
    )
  ;

  std::abort();
}

