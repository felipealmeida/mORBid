/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_INTERFACE_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/grammar/op_decl.hpp>
#include <morbid/idl_parser/grammar/attribute.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>

namespace std {

inline std::ostream& operator<<(std::ostream& os, std::vector<morbid::idl_parser::op_decl> o)
{
  return os << boost::make_iterator_range(o.begin(), o.end());
}

inline std::ostream& operator<<(std::ostream& os, std::vector< morbid::idl_parser::attribute> const& o)
{
  return os << boost::make_iterator_range(o.begin(), o.end());
}

}

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct interface_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::interface_def()
   , qi::locals< boost::wave::util::file_position_type
                 , std::vector<idl_parser::op_decl>
                 , std::vector<idl_parser::attribute> >
   , skipper<Iterator>
  >
{
  interface_definition()
    : interface_definition::base_type(start)
  {
    namespace spirit = boost::spirit;
    namespace qi = spirit::qi;
    namespace phoenix = boost::phoenix;
    using qi::_a; using qi::_b; using qi::_c;
    using qi::_1;

    start %= 
      &(token_position[qi::_a = qi::_1])
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value("interface")
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> qi::omit
      [
       -(
         token_id(boost::wave::T_COLON)
         >> (scoped_name % token_id(boost::wave::T_COMMA))
         [phoenix::at_c<6>(spirit::_val) = spirit::_1]
        )
      ]
      >> token_id(boost::wave::T_LEFTBRACE)
      >> qi::omit
      [
         *(
           (
            op_decl[phoenix::push_back(_b, _1)]
            | attribute[phoenix::push_back(_c, _1)]
           )
           >> token_id(boost::wave::T_SEMICOLON)
          )
      ]
      >> qi::attr(_b)
      >> qi::attr(_c)
      >> token_id(boost::wave::T_RIGHTBRACE)
      >> qi::attr(std::vector<wave_string>())
      >> qi::attr(qi::_a)
      >> qi::attr(true) // fully defined
      ;

    start.name("interface_def");
    // qi::debug(start);
  }

  grammar::scoped_name<Iterator> scoped_name;
  grammar::op_decl<Iterator> op_decl;
  grammar::attribute_definition<Iterator> attribute;
  boost::spirit::qi::rule<Iterator, idl_parser::interface_def()
                          , qi::locals< boost::wave::util::file_position_type
                                        , std::vector<idl_parser::op_decl>
                                        , std::vector<idl_parser::attribute> >
                          , skipper<Iterator> > start;
};

} } }

#endif
