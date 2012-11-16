/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_SEQUENCE_HPP
#define MORBID_IIOP_GRAMMAR_SEQUENCE_HPP

#include <morbid/iiop/grammar/integer.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace grammar {

template <typename Iterator, typename T>
struct sequence : qi::grammar
<Iterator, std::vector<T>(bool), qi::locals<boost::uint_t<32u>::least> >
{
  sequence()
    : sequence::base_type(start)
  {
    using qi::_r1; using qi::_a;
    using qi::_1;
    
    start =
      qi::omit
      [
       dword(_r1)[_a = _1]
      ]
      >> qi::repeat(_a)[qi::char_]
     ;
    start.name("sequence");
    qi::debug(start);
    // dword.name("dword");
    // qi::debug(dword);
  }

  grammar::dword<Iterator> dword;
  qi::rule<Iterator, std::vector<T>(bool)
           , qi::locals<boost::uint_t<32u>::least> > start;
};

} } }

#endif
