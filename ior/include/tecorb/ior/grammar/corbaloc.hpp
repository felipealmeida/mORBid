/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IOR_GRAMMAR_IOR_HPP
#define TECORB_IOR_GRAMMAR_IOR_HPP

#include <tecorb/ior/iiop_profile.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace ior { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct corbaloc : qi::grammar
<Iterator, iiop_profile()
 , qi::locals<std::vector<char>, unsigned short, std::vector<char> > >
{
  corbaloc()
    : corbaloc::base_type(start)
  {
    using qi::_val; using qi::_1;
    using qi::_a; using qi::_b; using qi::_c;

    start =
      "corbaloc:"
      >> (
          -qi::lit("iiop")
          >> ":"
          >> -(qi::uint_ >> "." >> qi::uint_ >> "@")
          >> (+(qi::alnum | qi::char_('_') | qi::char_('-') | qi::char_('.')))[_a = _1]
          >> -(":" >> qi::ushort_[_b = _1])
         ) % ","
      >> "/" >> (+qi::char_)[_c = _1]
      [_val = phoenix::construct<iiop_profile>(_a, _b, _c)]
      ;
  }

  qi::rule<Iterator, iiop_profile()
           , qi::locals<std::vector<char>, unsigned short, std::vector<char> > > start;
};

} } }

#endif
