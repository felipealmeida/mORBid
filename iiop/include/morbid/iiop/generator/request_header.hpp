/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_REQUEST_HEADER_HPP
#define TECORB_IIOP_GENERATOR_REQUEST_HEADER_HPP

#include <morbid/iiop/request_header.hpp>
#include <morbid/iiop/generator/integer.hpp>
#include <morbid/iiop/generator/string.hpp>
#include <morbid/iiop/generator/sequence.hpp>
#include <morbid/iiop/generator/service_context_list.hpp>
#include <morbid/iiop/generator/align.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace generator {

template <typename OutputIterator>
struct request_header : karma::grammar<OutputIterator, iiop::request_header(bool)>
{
  request_header()
    : request_header::base_type(start)
  {
    namespace phoenix = boost::phoenix;
    using karma::_1;
    using karma::_r1; using karma::_r2;
    using karma::_val;
    using phoenix::ref;

    start %= 
      alignable[
      service_context_list(_r1)
      << dword(_r1)
      << karma::char_ << '\0' << '\0' << '\0'
      << generator::align(4u) << sequence(_r1)
      << generator::align(4u) << string(_r1)
      << generator::align(4u) << sequence(_r1)
      ]
      ;
    start.name("request_header");
    karma::debug(start);
  }

  generator::string<OutputIterator> string;
  generator::sequence<OutputIterator, char> sequence;
  generator::service_context_list<OutputIterator> service_context_list;
  generator::dword<OutputIterator> dword;
  karma::rule<OutputIterator, iiop::request_header(bool)> start;
};

} } }

#endif
