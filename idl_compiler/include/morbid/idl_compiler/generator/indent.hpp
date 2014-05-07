/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 * 
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_INDENT_HPP
#define MORBID_IDL_COMPILER_INDENT_HPP

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct indent : karma::grammar<OutputIterator>
{
  indent()
    : indent::base_type(start)
  {
    start = karma::space << karma::space;
  }

  karma::rule<OutputIterator> start;
};

} } }

#endif
