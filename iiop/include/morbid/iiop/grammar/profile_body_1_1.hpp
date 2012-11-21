/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_PROFILE_BODY_1_1_HPP
#define MORBID_IIOP_GRAMMAR_PROFILE_BODY_1_1_HPP

#include <morbid/iiop/profile_body.hpp>
#include <morbid/iiop/grammar/align.hpp>
#include <morbid/iiop/grammar/string.hpp>
#include <morbid/iiop/grammar/sequence.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct profile_body_1_1 : qi::grammar<Iterator, iiop::profile_body(Iterator)
                        , qi::locals<bool, boost::uint_t<32u>::least> >
{
  profile_body_1_1()
    : profile_body_1_1::base_type(start)
  {
    using qi::_r1; using qi::_1; using qi::_a; using qi::_val;
    using qi::_r2;

    start %= 
      qi::omit
      [
       qi::char_[_a = _1]
       >> qi::char_ // major version
       >> qi::char_ // minor version
      ]
      >> grammar::align(_r1, 4u)
      >> string(_a)
      >> grammar::align(_r1, 2u)
      >> word(_a)
      >> grammar::align(_r1, 4u)
      >> octet_sequence(_a)
      /* >> tagged_components */
      ;
  }

  grammar::word<Iterator> word;
  grammar::sequence<Iterator, char> octet_sequence;
  grammar::string<Iterator> string;
  qi::rule<Iterator, iiop::profile_body(Iterator)
           , qi::locals<bool, boost::uint_t<32u>::least> > start;
};

} } }

#endif
