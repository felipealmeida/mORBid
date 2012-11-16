/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_MODULE_OPEN_HPP
#define MORBID_IDL_PARSER_GRAMMAR_MODULE_OPEN_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct module_open : boost::spirit::qi::grammar
 <Iterator, std::string(), skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  module_open(TokenDef const& tok)
    : module_open::base_type(start)
  {
    start %= qi::omit[tok.module_keyword]
      >> tok.identifier >> qi::omit['{'];
  }

  boost::spirit::qi::rule<Iterator, std::string(), skipper<Iterator> > start;
};

} } }

#endif
