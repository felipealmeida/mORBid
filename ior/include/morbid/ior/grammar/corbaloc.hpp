/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GRAMMAR_CORBALOC_HPP
#define MORBID_IOR_GRAMMAR_CORBALOC_HPP

#include <morbid/iiop/profile_body.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace ior { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct corbaloc : qi::grammar
<Iterator, iiop::profile_body()
 , qi::locals<std::string, unsigned short, std::vector<char> > >
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
          >> qi::as_string[(+(qi::alnum | qi::char_('_') | qi::char_('-') | qi::char_('.')))][_a = _1]
          >> -(":" >> qi::ushort_[_b = _1])
         ) % ","
      >> "/" >> (+qi::char_)[_c = _1]
      [_val = phoenix::construct<iiop::profile_body>(_a, _b, _c)]
      ;
  }

  qi::rule<Iterator, iiop::profile_body()
           , qi::locals<std::string, unsigned short, std::vector<char> > > start;
};

} } }

#endif
