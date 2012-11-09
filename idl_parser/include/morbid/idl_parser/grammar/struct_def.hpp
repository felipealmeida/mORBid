/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_GRAMMAR_STRUCT_DEF_HPP
#define TECORB_IDL_PARSER_GRAMMAR_STRUCT_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/struct_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct struct_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::struct_def<Iterator>()
   , skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  struct_definition(TokenDef const& tok)
    : struct_definition::base_type(start)
    , type_spec(tok)
  {
    start %= qi::omit[tok.struct_keyword]
      >> tok.identifier
      >> qi::omit[qi::char_('{')]
      >> qi::omit[*(type_spec >> tok.identifier >> -(',' >> tok.identifier) >> ';')]
      >> qi::omit[qi::char_('}')]
      ;
  }

  grammar::type_spec<Iterator> type_spec;
  boost::spirit::qi::rule<Iterator, idl_parser::struct_def<Iterator>()
                          , skipper<Iterator> > start;
};

} } }

#endif
