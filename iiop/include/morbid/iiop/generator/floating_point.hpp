/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GENERATOR_FLOATING_POINT_HPP
#define MORBID_IIOP_GENERATOR_FLOATING_POINT_HPP

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct float_ : karma::grammar<OutputIterator, float(bool)>
{
  float_() : float_::base_type(start)
  {
    using karma::_r1; using karma::_1; using karma::_val;
    start %= (karma::eps(_r1) << karma::little_bin_float)
      | karma::big_bin_float
      ;
    start.name("float");
    karma::debug(start);
  }

  karma::rule<OutputIterator, float(bool)> start;
};

template <typename OutputIterator>
struct double_ : karma::grammar<OutputIterator, double(bool)>
{
  double_() : double_::base_type(start)
  {
    using karma::_r1; using karma::_1; using karma::_val;
    start %= (karma::eps(_r1) << karma::little_bin_double)
      | karma::big_bin_double
      ;
    start.name("double");
    karma::debug(start);
  }

  karma::rule<OutputIterator, double(bool)> start;
};

} } }

#endif
