/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_TYPEDEF_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_TYPEDEF_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/grammar/integer_literal.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct typedef_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::typedef_def()
   , qi::locals<boost::wave::util::file_position_type>
   , skipper<Iterator> >
{
  typedef_definition()
    : typedef_definition::base_type(start)
  {
    start %=
      &(token_position[qi::_a = qi::_1])
      >> token_id(boost::wave::T_TYPEDEF)
      >> type_spec
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> 
      -(token_id(boost::wave::T_LEFTBRACKET)
        >> &token_category(boost::wave::IntegerLiteralTokenType)
        >> integer_literal
        >> token_id(boost::wave::T_RIGHTBRACKET)
       )
      >> qi::attr(qi::_a)
      ;

    start.name("typedef_def");
    qi::debug(start);
  }

  grammar::integer_literal_definition<Iterator> integer_literal;
  grammar::type_spec<Iterator> type_spec;
  boost::spirit::qi::rule<Iterator, idl_parser::typedef_def()
                          , qi::locals<boost::wave::util::file_position_type>
                          , skipper<Iterator> > start;
};

} } }

#endif
