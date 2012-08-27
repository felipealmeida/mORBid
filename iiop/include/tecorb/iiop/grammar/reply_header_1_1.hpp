/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IIOP_GRAMMAR_REPLY_HEADER_1_1_HPP
#define TECORB_IIOP_GRAMMAR_REPLY_HEADER_1_1_HPP

#include <tecorb/iiop/reply_header.hpp>
#include <tecorb/iiop/grammar/service_context_list.hpp>
#include <tecorb/iiop/grammar/integer.hpp>
#include <tecorb/iiop/grammar/align.hpp>
#include <tecorb/iiop/grammar/sequence.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <boost/integer.hpp>

namespace tecorb { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct reply_header_1_1 : qi::grammar
<Iterator, iiop::reply_header(Iterator, bool)>
{
  reply_header_1_1()
    : reply_header_1_1::base_type(start)
  {
    using qi::_r1;
    using qi::_r2;
    using qi::_1;
    using qi::_val;

    start %=
      service_context_list_grammar(_r2)
      >> align(_r1, 4) >> dword(_r2)
      >> dword(_r2)
      ;
    start.name("reply_header_1_1");
    qi::debug(start);
  }

  grammar::dword<Iterator> dword;
  service_context_list<Iterator> service_context_list_grammar;
  qi::rule<Iterator, iiop::reply_header(Iterator, bool)> start;
};

} } }

#endif
