/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP
#define TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/grammar/op_decl.hpp>

namespace std {

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, std::vector<morbid::idl_parser::op_decl<Iterator> > o)
{
  return os << boost::make_iterator_range(o.begin(), o.end());
}

}

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct interface_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::interface_def<Iterator>()
   , qi::locals<boost::iterator_range<typename Iterator::base_iterator_type>
                , std::vector<idl_parser::op_decl<Iterator> >
               >
   , skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  interface_definition(TokenDef const& tok)
    : interface_definition::base_type(start)
    , op_decl(tok)
  {
    namespace qi = boost::spirit::qi;
    namespace lex = boost::spirit::lex;
    namespace phoenix = boost::phoenix;

    typedef idl_parser::interface_def<Iterator> return_type;

    start %= qi::omit[tok.interface_keyword]
      >> tok.identifier
      >> qi::omit
      [
       -(
         ':'
         >> (tok.identifier % ',')
        )
      ]
      >> qi::omit[qi::char_('{')]
      >> *(op_decl >> qi::omit[qi::char_(';')])
      >> qi::omit[qi::char_('}')]
      ;

    start.name("interface_def");
    qi::debug(start);
  }

  grammar::op_decl<Iterator> op_decl;
  boost::spirit::qi::rule<Iterator, idl_parser::interface_def<Iterator>()
                          , qi::locals<boost::iterator_range<base_iterator>
                                       , std::vector<idl_parser::op_decl<Iterator> > >
                          , skipper<Iterator> > start;
};

} } }

#endif
