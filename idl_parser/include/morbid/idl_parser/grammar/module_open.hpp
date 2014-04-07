/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_MODULE_OPEN_HPP
#define MORBID_IDL_PARSER_GRAMMAR_MODULE_OPEN_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/wave/token_ids.hpp>

#include <morbid/idl_parser/token.hpp>
#include <morbid/idl_parser/typedef_def.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct module_open : boost::spirit::qi::grammar
<Iterator, BOOST_WAVE_STRINGTYPE(), skipper<Iterator> >
{
  module_open()
    : module_open::base_type(start)
  {
    using qi::_1;
    using qi::_val;

    start %=
      qi::omit[&token_id(boost::wave::T_IDENTIFIER)]
      >> qi::omit[token_value("module")]
      >> qi::omit[&token_id(boost::wave::T_IDENTIFIER)]
      >> token_value
      >> qi::omit[token_value("{")]
      ;

    start.name("module_open");
    // qi::debug(start);
  }

  boost::spirit::qi::rule<Iterator, BOOST_WAVE_STRINGTYPE(), skipper<Iterator> > start;
};

} } }

#endif
