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

#include <tecorb/idl_parser/tokenizer.hpp>
#include <tecorb/idl_parser/op_decl.hpp>

namespace tecorb { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct op_decl : qi::grammar
<Iterator, idl_parser::op_decl<Iterator>()
 , qi::locals<boost::iterator_range
                 <typename Iterator::base_iterator_type>
               , boost::iterator_range
                 <typename Iterator::base_iterator_type>
               , boost::iterator_range
                 <typename Iterator::base_iterator_type>
               , boost::iterator_range
                 <typename Iterator::base_iterator_type>
               , boost::iterator_range
                 <typename Iterator::base_iterator_type>
                 >
 , skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  op_decl()
    : op_decl::base_type(start)
  {
    typedef idl_parser::op_decl<Iterator> return_type;

    start = qi::token(token_types::identifier)[qi::_a = qi::_1]
      >> qi::token(token_types::identifier)[qi::_b = qi::_1]
      >> qi::token(token_types::open_parenthesis)
      >> -(
           (qi::token(token_types::in_keyword)
            | qi::token(token_types::out_keyword)
            | qi::token(token_types::inout_keyword))
           [qi::_c = qi::_1]
           >> qi::token(token_types::identifier)[qi::_d = qi::_1]
           >> qi::token(token_types::identifier)[qi::_e = qi::_1]
           >> qi::eps // add new push
           >> *(
                qi::token(token_types::comma)
                 >> (qi::token(token_types::in_keyword)
                     | qi::token(token_types::out_keyword)
                     | qi::token(token_types::inout_keyword))
                >> qi::token(token_types::identifier)
                >> qi::token(token_types::identifier))
           )
      >> qi::token(token_types::close_parenthesis)
      >> qi::eps[qi::_val = phoenix::construct<return_type>(qi::_a, qi::_b)]
      ;
    start.name("op_decl");
    qi::debug(start);
  }

  qi::rule<Iterator, idl_parser::op_decl<Iterator>()
           , qi::locals<boost::iterator_range<base_iterator> // return
                        , boost::iterator_range<base_iterator> // name
                        , boost::iterator_range<base_iterator> // in/out keyword
                        , boost::iterator_range<base_iterator> // typename
                        , boost::iterator_range<base_iterator> // arg type
                                                >
           , skipper<Iterator> > start;
};

} } }

#endif
