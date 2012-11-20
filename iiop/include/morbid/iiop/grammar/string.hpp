/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_STRING_HPP
#define MORBID_IIOP_GRAMMAR_STRING_HPP

#include <morbid/iiop/grammar/integer.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace grammar {

template <typename Iterator>
struct string : qi::grammar
<Iterator, std::string(bool), qi::locals<boost::uint_t<32u>::least> >
{
  string()
    : string::base_type(start)
  {
    using qi::_r1; using qi::_a;
    using qi::_1;
    
    start %=
      qi::omit [ dword(_r1)[_a = _1] ]
      >> qi::eps(_a > 0u)
      >> qi::repeat(_a - 1)[qi::char_]
      >> qi::omit[qi::char_('\0')]
     ;
    start.name("string");
    qi::debug(start);
    // dword.name("dword");
    // qi::debug(dword);
  }

  grammar::dword<Iterator> dword;
  qi::rule<Iterator, std::string(bool)
           , qi::locals<boost::uint_t<32u>::least> > start;
};

} } }

#endif
