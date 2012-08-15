#ifndef TECORB_IDL_PARSER_TOKENIZER_HPP
#define TECORB_IDL_PARSER_TOKENIZER_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>

#include <boost/spirit/include/lex_lexertl.hpp>

namespace tecorb { namespace idl_parser {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

namespace token_types {

const std::size_t identifier = 1;
const std::size_t blank = 3;
const std::size_t interface_keyword = 4;
const std::size_t colon = 5;
const std::size_t comma = 6;
const std::size_t open_curly_bracket = 7;
const std::size_t close_curly_bracket = 8;
const std::size_t open_parenthesis = 9;
const std::size_t close_parenthesis = 10;
const std::size_t in_keyword = 11;
const std::size_t out_keyword = 12;
const std::size_t inout_keyword = 13;
const std::size_t semicolon = 14;

}

template <typename Lexer>
struct tokens : boost::spirit::lex::lexer<Lexer>
{
  tokens()
  {
    this->self.add
      ("interface", token_types::interface_keyword)
      ("in", token_types::in_keyword)
      ("out", token_types::out_keyword)
      ("inout", token_types::inout_keyword)
      ("[a-zA-Z][a-zA-Z0-9]*", token_types::identifier)
      (':', token_types::colon)
      (',', token_types::comma)
      ('{', token_types::open_curly_bracket)
      ('}', token_types::close_curly_bracket)
      ('(', token_types::open_parenthesis)
      (')', token_types::close_parenthesis)
      (';', token_types::semicolon)
      ("(\t|\r|\n| )+", token_types::blank)
      ;
  }
};

template <typename Iterator>
struct skipper : boost::spirit::qi::grammar<Iterator>
{
  skipper()
    : skipper::base_type(start)
  {
    start = boost::spirit::qi::token(token_types::blank);
  }
  boost::spirit::qi::rule<Iterator> start;
};

} }

#endif
