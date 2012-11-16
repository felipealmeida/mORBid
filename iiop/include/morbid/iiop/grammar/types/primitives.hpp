/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_TYPES_PRIMITIVES_HPP
#define MORBID_IIOP_GRAMMAR_TYPES_PRIMITIVES_HPP

#include <morbid/iiop/grammar/floating_point.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct boolean : qi::grammar<Iterator, bool()>
{
  boolean() : boolean::base_type(start)
  {
    start %= qi::char_;
  }
  qi::rule<Iterator, bool()> start;
};

template <typename Iterator>
struct char_ : qi::grammar<Iterator, char()>
{
  char_() : char_::base_type(start)
  {
    start %= qi::char_;
  }
  qi::rule<Iterator, char()> start;
};

template <typename Iterator>
struct octet : qi::grammar<Iterator, unsigned char()>
{
  octet() : octet::base_type(start)
  {
    start %= qi::char_;
  }
  qi::rule<Iterator, unsigned char()> start;
};

} } }

#endif
