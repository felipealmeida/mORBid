/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_PARSER_TOKENIZER_HPP
#define TECORB_IDL_PARSER_TOKENIZER_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>

#include <boost/spirit/include/lex_lexertl.hpp>

namespace tecorb { namespace idl_parser {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

// namespace token_types {

// const std::size_t identifier = 1;
// const std::size_t blank = 3;
// const std::size_t interface_keyword = 4;
// const std::size_t colon = 5;
// const std::size_t comma = 6;
// const std::size_t open_curly_bracket = 7;
// const std::size_t close_curly_bracket = 8;
// const std::size_t open_parenthesis = 9;
// const std::size_t close_parenthesis = 10;
// const std::size_t in_keyword = 11;
// const std::size_t out_keyword = 12;
// const std::size_t inout_keyword = 13;
// const std::size_t semicolon = 14;

// }

struct set_lexer_state
{
  set_lexer_state(char const* state)
    : state(state) {}

  template <typename Iterator, typename Context>
  void operator()(Iterator const&, Iterator const&
                  , BOOST_SCOPED_ENUM(boost::spirit::lex::pass_flags)&
                  , std::size_t&, Context& ctx) const
  {
    ctx.set_state_name(state);
  }

  char const* state;
};

template <typename Lexer>
struct tokens : lex::lexer<Lexer>
{
  tokens()
  {
    identifier = "[a-zA-Z][a-zA-Z0-9_]*";
    interface_keyword = "interface";
    in_keyword = "in";
    out_keyword = "out";
    inout_keyword = "inout";
    blanks = "[\r\n\t ]+";
    cppcomment = "\\/\\/[^\n]*\n";
    any = ".";
    comment_any = ".";
    ccomment = "\\/\\*";
    endccomment = "\\*\\/";

    this->self
      = lex::token_def<>('(')
      | ')' | '{' | '}' | ';' | ':' | in_keyword
      | interface_keyword | out_keyword | inout_keyword
      | identifier | blanks | ccomment | cppcomment
      | any
      ;
    this->self("COMMENT")
      = endccomment
      | comment_any
      ;
  }

  lex::token_def<lex::omit> ccomment, cppcomment, endccomment, interface_keyword, blanks
    , comment_any;
  lex::token_def<std::string> identifier, in_keyword, out_keyword, inout_keyword;
  lex::token_def<> any;
};

template <typename Iterator>
struct skipper : qi::grammar<Iterator>
{
  template <typename TokenDef>
  skipper(TokenDef const& tok)
    : skipper::base_type(start)
  {
    start = 
      +(tok.blanks | tok.cppcomment
      | (
         tok.ccomment > qi::in_state("COMMENT")
         [
          *tok.comment_any
          > tok.endccomment
         ]
        )
       )
       ;
  }
  
  qi::rule<Iterator> start;
};

} }

#endif
