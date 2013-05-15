/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_CONSTANT_HPP
#define MORBID_IDL_PARSER_GRAMMAR_CONSTANT_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>
#include <morbid/idl_parser/constant.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct constant_definition
  : qi::grammar<Iterator, constant(), skipper<Iterator> >
{
  constant_definition() : constant_definition::base_type(start)
  {
    using qi::_a; using qi::_b;
    using qi::_1; using qi::_val;
    namespace p = boost::phoenix;

    start %=
      token_id(boost::wave::T_CONST)
      >> type_spec
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> (token_id(boost::wave::T_EQUAL) | token_id(boost::wave::T_ASSIGN))
      >> token_value
      ;
    start.name("constant");
    qi::debug(start);
  }

  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, constant(), skipper<Iterator> > start;
};

} } }


#endif

