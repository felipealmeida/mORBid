/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_OP_DECL_HPP
#define MORBID_IDL_PARSER_GRAMMAR_OP_DECL_HPP

#include <morbid/idl_parser/grammar/type_spec.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

#include <morbid/idl_parser/op_decl.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct op_decl : qi::grammar
  <Iterator, idl_parser::op_decl()
   , qi::locals<boost::wave::util::file_position_type>
   , skipper<Iterator> >
{
  op_decl()
    : op_decl::base_type(start)
  {
    using qi::_val;

    start %= 
      &(token_position[qi::_a = qi::_1])
      >> qi::omit[-(&token_id(boost::wave::T_IDENTIFIER) >> token_value("oneway"))]
      >> type_spec
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      >> token_id(boost::wave::T_LEFTPAREN)
      >> (
          (&param >> (param % token_id(boost::wave::T_COMMA)))
          | qi::eps
         )
      >> token_id(boost::wave::T_RIGHTPAREN)
      >> -qi::omit
      [
       &token_id(boost::wave::T_IDENTIFIER)
       >> token_value("raises")
       >> token_id(boost::wave::T_LEFTPAREN)
       >> (scoped_name % token_id(boost::wave::T_COMMA))
       >> token_id(boost::wave::T_RIGHTPAREN)
      ]
      >> qi::attr(qi::_a)
      >> qi::attr(true)
      ;
    direction = 
      (&token_id(boost::wave::T_IDENTIFIER) >> token_value("in"))[_val = idl_parser::direction::in()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("out"))[_val = idl_parser::direction::out()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("inout"))[_val = idl_parser::direction::inout()]
      ;
    param %=
      &(token_position[qi::_a = qi::_1])
      >> direction
      >> type_spec
      >> token_id(boost::wave::T_IDENTIFIER)
      >> qi::attr(qi::_a)
      ;      
    start.name("op_decl");
    param.name("param");
    direction.name("direction");
    // qi::debug(start);
    // qi::debug(param);
    // qi::debug(direction);
  }

  grammar::scoped_name<Iterator> scoped_name;
  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, boost::variant<idl_parser::direction::in
                                    , idl_parser::direction::out
                                    , idl_parser::direction::inout>()
           , skipper<Iterator> > direction;
  qi::rule<Iterator, idl_parser::param_decl()
           , qi::locals<boost::wave::util::file_position_type>
           , skipper<Iterator> > param;
  qi::rule<Iterator, idl_parser::op_decl()
           , qi::locals<boost::wave::util::file_position_type>
           , skipper<Iterator> > start;
};

} } }

#endif
