/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_FLOATING_POINT_HPP
#define MORBID_IIOP_GRAMMAR_FLOATING_POINT_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct float_ : qi::grammar<Iterator, float(bool)>
{
  float_() : float_::base_type(start)
  {
    using qi::_r1;
    start %= (qi::eps(_r1) >> qi::little_bin_float)
      | qi::big_bin_float
      ;
    start.name("float");
    qi::debug(start);
  }

  qi::rule<Iterator, float(bool)> start;
};

template <typename Iterator>
struct double_ : qi::grammar<Iterator, double(bool)>
{
  double_() : double_::base_type(start)
  {
    using qi::_r1;
    start %= (qi::eps(_r1) >> qi::little_bin_double)
      | qi::big_bin_double
      ;
    start.name("double");
    qi::debug(start);
  }

  qi::rule<Iterator, double(bool)> start;
};

} } }

#endif
