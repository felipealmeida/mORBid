/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_EXCEPTION_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_EXCEPTION_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/exception_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct exception_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::exception_def<Iterator>()
   , skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  exception_definition(TokenDef const& tok)
    : exception_definition::base_type(start)
    , type_spec(tok)
  {
    start %= qi::omit[tok.exception_keyword]
      >> tok.identifier
      >> qi::omit[qi::char_('{')]
      >> qi::omit[qi::char_('}')]
      ;
  }

  grammar::type_spec<Iterator> type_spec;
  boost::spirit::qi::rule<Iterator, idl_parser::exception_def<Iterator>()
                          , skipper<Iterator> > start;
};

} } }

#endif
