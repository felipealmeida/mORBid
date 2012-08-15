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
const std::size_t symbol = 2;
const std::size_t blank = 3;
const std::size_t interface_keyword = 4;
const std::size_t colon = 5;
const std::size_t comma = 6;
const std::size_t open_curly_bracket = 7;
const std::size_t close_curly_bracket = 8;

}

template <typename Lexer>
struct tokens : boost::spirit::lex::lexer<Lexer>
{
  tokens()
  {
    this->self.add
      ("interface", token_types::interface_keyword)
      ("[a-zA-Z][a-zA-Z0-9]*", token_types::identifier)
      (':', token_types::colon)
      (',', token_types::comma)
      ('{', token_types::open_curly_bracket)
      ('}', token_types::close_curly_bracket)
      ("\\(|\\)\\;", token_types::symbol)
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
