#ifndef TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP
#define TECORB_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <tecorb/idl_parser/tokenizer.hpp>
#include <tecorb/idl_parser/interface_def.hpp>
#include <tecorb/idl_parser/grammar/op_decl.hpp>

namespace std {

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, std::vector<tecorb::idl_parser::op_decl<Iterator> > o)
{
  return os << boost::make_iterator_range(o.begin(), o.end());
}

}

namespace tecorb { namespace idl_parser { namespace grammar {

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

  interface_definition()
    : interface_definition::base_type(start)
  {
    namespace qi = boost::spirit::qi;
    namespace lex = boost::spirit::lex;
    namespace phoenix = boost::phoenix;

    typedef idl_parser::interface_def<Iterator> return_type;

    start = qi::token(token_types::interface_keyword)
      >> qi::token(token_types::identifier)
      [qi::_a = qi::_1]
      >> -(
           qi::token(token_types::colon)
           >> qi::token(token_types::identifier)
           >> *( qi::token(token_types::comma)
                 >> qi::token(token_types::identifier)))
      >> qi::token(token_types::open_curly_bracket)
      >> *( op_decl[phoenix::push_back(qi::_b, qi::_1)] >> qi::token(token_types::semicolon) )
      >> qi::token(token_types::close_curly_bracket)
      >> qi::eps[qi::_val = phoenix::construct<return_type>(qi::_a)]
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
