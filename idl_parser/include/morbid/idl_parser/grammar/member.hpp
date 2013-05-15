/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_MEMBER_HPP
#define MORBID_IDL_PARSER_GRAMMAR_MEMBER_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>
#include <morbid/idl_parser/struct_def.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct member_definition
  : qi::grammar<Iterator, std::vector<member>()
  , qi::locals<idl_parser::type_spec, std::vector<member>
               , boost::wave::util::file_position_type>
  , skipper<Iterator> >
{
  member_definition() : member_definition::base_type(start)
  {
    using qi::_a; using qi::_b; using qi::_c;
    using qi::_1; using qi::_val;
    namespace p = boost::phoenix;

    start =
      &token_position[_c = _1]
      >> type_spec[_a = _1]
      >> (
          (&token_id(boost::wave::T_IDENTIFIER)
           >> token_value)
          [
           p::push_back(_b, p::construct<member>(_a, _1, _c))
          ]
          % token_id(boost::wave::T_COMMA)
         )
      >> token_id(boost::wave::T_SEMICOLON)[_val = _b]
      ;
  }

  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, std::vector<member>()
           , qi::locals<idl_parser::type_spec, std::vector<member>
                        , boost::wave::util::file_position_type>
           , skipper<Iterator> > start;
};

} } }


#endif

