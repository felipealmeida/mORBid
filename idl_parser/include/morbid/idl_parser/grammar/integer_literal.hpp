/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_INTEGER_LITERAL_HPP
#define MORBID_IDL_PARSER_GRAMMAR_INTEGER_LITERAL_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct integer_literal_definition : boost::spirit::qi::grammar
  <Iterator, unsigned int(), skipper<Iterator> >
{
  integer_literal_definition()
    : integer_literal_definition::base_type(start)
  {
    start %=
      (&token_id(boost::wave::T_OCTALINT) >> parse_token_value[qi::oct])
      | (&token_id(boost::wave::T_DECIMALINT) >> parse_token_value[qi::uint_])
      | (&token_id(boost::wave::T_HEXAINT) >> parse_token_value[qi::hex])
      | (&token_id(boost::wave::T_INTLIT) >> parse_token_value[qi::uint_])
      ;

    start.name("integer_literal_definition");
    qi::debug(start);
  }

  boost::spirit::qi::rule<Iterator, unsigned int(), skipper<Iterator> > start;
};

} } }

#endif
