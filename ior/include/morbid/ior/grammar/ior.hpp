/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_IOR_HPP
#define MORBID_IOR_GRAMMAR_IOR_HPP

#include <morbid/ior/grammar/tagged_profile.hpp>

#include <boost/fusion/include/at.hpp>
#include <boost/spirit/home/support.hpp>

#include <iostream>

namespace morbid { namespace ior { namespace grammar {

namespace fusion = boost::fusion;
namespace spirit = boost::spirit;

template <typename Domain, typename Iterator, typename Attr>
struct ior : giop::grammar<Domain, Iterator, Attr(giop::endian)>
{
  template <typename Profile>
  ior(Profile const& profile) : ior::base_type(start)
  {
    init(profile);
  }

  template <typename Profile>
  void init(Profile const& profile)
  {
    start = 
       giop::string
       & giop::sequence
       [
        profile
       ]
      ;
  }

  giop::rule<Domain, Iterator, Attr(giop::endian)> start;
};

} } }

#endif
