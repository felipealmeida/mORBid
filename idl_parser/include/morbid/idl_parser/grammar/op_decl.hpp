/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_GRAMMAR_OP_DECL_HPP
#define TECORB_IDL_PARSER_GRAMMAR_OP_DECL_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/op_decl.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct op_decl : qi::grammar
<Iterator, idl_parser::op_decl<Iterator>(), skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  op_decl(TokenDef const& tok)
    : op_decl::base_type(start)
  {
    using qi::_val;

    typedef idl_parser::op_decl<Iterator> return_type;

    start %= tok.identifier
      >> tok.identifier
      >> qi::omit[qi::char_('(')]
      >> (
          (&param >> (param % ","))
          | qi::eps
         )
      >> qi::omit[qi::char_(')')]
      >> qi::attr(true)
      ;
    param %=
      (tok.in_keyword | tok.out_keyword | tok.inout_keyword)
      >> tok.identifier
      >> qi::omit[tok.identifier]
      ;      
    start.name("op_decl");
    param.name("param");
    qi::debug(start);
    qi::debug(param);
  }

  qi::rule<Iterator, idl_parser::param_decl()
           , skipper<Iterator> > param;
  qi::rule<Iterator, idl_parser::op_decl<Iterator>()
           , skipper<Iterator> > start;
};

} } }

#endif
