/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_EXCEPTION_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_EXCEPTION_DEF_HPP

#include <morbid/idl_parser/grammar/member.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>
#include <morbid/idl_parser/exception_def.hpp>
#include <morbid/idl_parser/token.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct exception_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::exception_def(), skipper<Iterator> >
{
  exception_definition()
    : exception_definition::base_type(start)
  {
    start %=
      qi::omit[&token_id(boost::wave::T_IDENTIFIER)]
      >> qi::omit[token_value("exception")]
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> qi::omit[token_id(boost::wave::T_LEFTBRACE)]
      >> *member
      >> qi::omit[token_id(boost::wave::T_RIGHTBRACE)]
      ;
    start.name("exception_definition");
    // qi::debug(start);
  }

  grammar::member_definition<Iterator> member;
  grammar::type_spec<Iterator> type_spec;
  boost::spirit::qi::rule<Iterator, idl_parser::exception_def()
                          , skipper<Iterator> > start;
};

} } }

#endif
