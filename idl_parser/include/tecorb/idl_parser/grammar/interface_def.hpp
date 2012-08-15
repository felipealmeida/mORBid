#ifndef TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP
#define TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <tecorb/idl_parser/tokenizer.hpp>
#include <tecorb/idl_parser/interface_def.hpp>

namespace tecorb { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct interface_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::interface_def<Iterator>()
   , skipper<Iterator> >
{
  interface_definition()
    : interface_definition::base_type(start)
  {
    namespace qi = boost::spirit::qi;
    namespace lex = boost::spirit::lex;
    namespace phoenix = boost::phoenix;

    typedef idl_parser::interface_def<Iterator> return_type;

    start = qi::token(token_types::interface_keyword)
      >> qi::token(token_types::identifier)
      [qi::_val = phoenix::construct<return_type>(qi::_1)]
      >> -(
           qi::token(token_types::colon)
           >> qi::token(token_types::identifier)
           >> *( qi::token(token_types::comma)
                 >> qi::token(token_types::identifier)))
      >> qi::token(token_types::open_curly_bracket)
      >> qi::token(token_types::close_curly_bracket)
      ;

    start.name("interface_definition entry-point");
    qi::debug(start);
  }

  boost::spirit::qi::rule<Iterator, idl_parser::interface_def<Iterator>()
                          , skipper<Iterator> > start;
};

} } }

#endif
