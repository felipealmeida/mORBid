/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_INTERFACE_FORWARD_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_INTERFACE_FORWARD_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/op_decl.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>

namespace std {

// inline std::ostream& operator<<(std::ostream& os, std::vector<morbid::idl_parser::op_decl> o)
// {
//   return os << boost::make_iterator_range(o.begin(), o.end());
// }

}

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct interface_forward_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::interface_def()
   , qi::locals<boost::wave::util::file_position_type>
   , skipper<Iterator>
  >
{
  interface_forward_definition()
    : interface_forward_definition::base_type(start)
  {
    namespace qi = boost::spirit::qi;
    namespace phoenix = boost::phoenix;

    start %=
      &(token_position[qi::_a = qi::_1])
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value("interface")
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> qi::attr(std::vector<idl_parser::op_decl>())
      >> qi::attr(std::vector<idl_parser::attribute>())
      >> qi::attr(std::vector<wave_string>())
      >> qi::attr(qi::_a)
      >> qi::attr(false) // fully defined
      ;

    start.name("interface_forward_def");
    qi::debug(start);
  }

  boost::spirit::qi::rule<Iterator, idl_parser::interface_def()
                          , qi::locals<boost::wave::util::file_position_type>
                          , skipper<Iterator> > start;
};

} } }

#endif
