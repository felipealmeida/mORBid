/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_TAGGED_PROFILE_HPP
#define MORBID_IOR_GRAMMAR_TAGGED_PROFILE_HPP

#include <morbid/ior/tagged_profile.hpp>
#include <morbid/iiop/grammar/integer.hpp>
#include <morbid/iiop/grammar/sequence.hpp>
#include <morbid/iiop/grammar/align.hpp>

#include <iostream>

namespace morbid { namespace ior { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct tagged_profiles : qi::grammar<Iterator, std::vector<morbid::ior::tagged_profile>(Iterator, bool)
                       , qi::locals<boost::uint_t<32u>::least> >
{
  tagged_profiles() : tagged_profiles::base_type(start)
  {
    using qi::_r1; using qi::_1; using qi::_val; using qi::_a;
    using qi::_r2;

    start %=
      qi::omit[dword(_r2)[_a = _1]]
      >> qi::repeat(_a)
      [
       iiop::grammar::align(_r1, 4u)
       >> dword(_r2)
       >> octet_sequence(_r2)
      ]
      ;
  }

  iiop::grammar::dword<Iterator> dword;
  iiop::grammar::sequence<Iterator, char> octet_sequence;
  qi::rule<Iterator, std::vector<morbid::ior::tagged_profile>(Iterator, bool)
    , qi::locals<boost::uint_t<32u>::least> > start;
};

} } }

#endif
