/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_REPLY_HEADER_HPP
#define TECORB_IIOP_GENERATOR_REPLY_HEADER_HPP

#include <morbid/iiop/generator/integer.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace generator {

template <typename OutputIterator>
struct reply_header : karma::grammar<OutputIterator, void(bool, unsigned int)>
{
  reply_header()
    : reply_header::base_type(start)
  {
    using karma::_r1;
    using karma::_r2;
    using karma::_1;

    start = dword(_r1)[_1 = 0u]
      << dword(_r1)[_1 = _r2]
      << dword(_r1)[_1 = 0u]
      ;
  }

  generator::dword<OutputIterator> dword;
  karma::rule<OutputIterator, void(bool, unsigned int)> start;
};

} } }

#endif
