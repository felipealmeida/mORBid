/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_PROFILE_BODY_1_1_HPP
#define MORBID_IOR_GRAMMAR_PROFILE_BODY_1_1_HPP

#include <morbid/giop/grammar.hpp>

#include <iostream>

namespace morbid { namespace iiop { namespace grammar {

template <typename Domain, typename Iterator, typename Attr>
struct profile_body_1_1 : giop::grammar<Domain, Iterator, Attr(giop::endian)>
{
  profile_body_1_1() : profile_body_1_1::base_type(start)
  {
    start =
      // giop::octet
      // & giop::octet
      /*&*/ giop::string
      & giop::ushort_
      & giop::sequence[giop::octet]
      ;
  }

  giop::rule<Domain, Iterator, Attr(giop::endian)> start;
};

} } }

#endif
