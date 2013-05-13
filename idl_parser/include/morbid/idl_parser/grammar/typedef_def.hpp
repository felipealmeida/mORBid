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
#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct typedef_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::typedef_def<Iterator>()>
{
  typedef_definition()
    : typedef_definition::base_type(start)
  {
    // start %= qi::omit[tok.typedef_keyword]
    //   >> type_spec
    //   >> tok.identifier;

    // // start.name("typedef_def");
    // // qi::debug(start);
  }

  grammar::type_spec<Iterator> type_spec;
  boost::spirit::qi::rule<Iterator, idl_parser::typedef_def<Iterator>()> start;
};

} } }

#endif
