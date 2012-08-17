/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GRAMMAR_SERVICE_CONTEXT_LIST_HPP
#define TECORB_IIOP_GRAMMAR_SERVICE_CONTEXT_LIST_HPP

#include <tecorb/iiop/service_context_list.hpp>
#include <tecorb/iiop/grammar/integer.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <boost/integer.hpp>

namespace tecorb { namespace iiop { namespace grammar {

template <typename Iterator>
struct service_context_list : qi::grammar
  <Iterator, iiop::service_context_list(bool), qi::locals<boost::uint_t<32u>::least> >
{
  
  service_context_list()
    : service_context_list::base_type(start)
  {
    using qi::_a;
    using qi::_1;
    using qi::_val;
    using qi::_r1;

    start %=
      qi::omit
      [
       dword(_r1)[_a = _1]
      ]
      >> qi::eps(_a == 0u)
      // [_1 = 0]
      // >> qi::repeat(_a)
      // [
       
      // ]
      ;
    start.name("service_context_list");
    qi::debug(start);
  }

  grammar::dword<Iterator> dword;
  qi::rule<Iterator, iiop::service_context_list(bool), qi::locals<boost::uint_t<32u>::least> > start;
};

} } }

#endif

