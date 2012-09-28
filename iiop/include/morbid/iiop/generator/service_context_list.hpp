/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GENERATOR_SERVICE_CONTEXT_LIST_HPP
#define TECORB_IIOP_GENERATOR_SERVICE_CONTEXT_LIST_HPP

#include <morbid/iiop/generator/integer.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace iiop { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct service_context_list : karma::grammar
  <OutputIterator, iiop::service_context_list(bool)>
{
  service_context_list()
    : service_context_list::base_type(start)
  {
    using karma::_r1;
    using karma::_1;
    using karma::_val;

    start = dword(_r1)[_1 = 0u];
    start.name("service_context_list");
    karma::debug(start);
  }

  generator::dword<OutputIterator> dword;
  karma::rule<OutputIterator, iiop::service_context_list(bool)> start;
};

} } }

#endif
