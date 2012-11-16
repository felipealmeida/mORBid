/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_GRAMMAR_REQUEST_HEADER_1_1_HPP
#define MORBID_IIOP_GRAMMAR_REQUEST_HEADER_1_1_HPP

#include <morbid/iiop/request_header.hpp>
#include <morbid/iiop/grammar/service_context_list.hpp>
#include <morbid/iiop/grammar/integer.hpp>
#include <morbid/iiop/grammar/align.hpp>
#include <morbid/iiop/grammar/sequence.hpp>
#include <morbid/iiop/grammar/string.hpp>

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <boost/integer.hpp>

namespace morbid { namespace iiop { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct request_header_1_1 : qi::grammar
<Iterator, iiop::request_header(Iterator, bool)
 , qi::locals<boost::uint_t<32u>::least> >
{
  request_header_1_1()
    : request_header_1_1::base_type(start)
  {
    using qi::_r1;
    using qi::_r2;
    using qi::_a;
    using qi::_1;
    using qi::_val;

    start %=
      service_context_list_grammar(_r2)
      >> align(_r1, 4) >> dword(_r2)
      >> qi::char_
      >> align(_r1, 4) >> octet_sequence(_r2) /* object_key */
      >> align(_r1, 4) >> string(_r2) /* operation */
      >> align(_r1, 4) >> octet_sequence(_r2) /* requesting principal */
      ;
    start.name("request_header_1_1");
    qi::debug(start);
  }

  grammar::sequence<Iterator, char> octet_sequence;
  grammar::string<Iterator, char> string;
  grammar::dword<Iterator> dword;
  service_context_list<Iterator> service_context_list_grammar;
  qi::rule<Iterator, iiop::request_header(Iterator, bool)
           , qi::locals<boost::uint_t<32u>::least> > start;
};

} } }

#endif
