/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_ATTR_DECL_HPP
#define MORBID_IDL_PARSER_GRAMMAR_ATTR_DECL_HPP

#include <morbid/idl_parser/grammar/type_spec.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/attr_decl.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct attr_decl : qi::grammar
<Iterator, idl_parser::attr_decl<Iterator>()>
{
  attr_decl()
    : attr_decl::base_type(start)
  {
    using qi::_val;

    typedef idl_parser::op_decl<Iterator> return_type;

    // start %= type_spec
    //   >> tok.identifier
    //   >> qi::omit['(']
    //   >> (
    //       (&param >> (param % ","))
    //       | qi::eps
    //      )
    //   >> qi::omit[')']
    //   >> -qi::omit
    //   [
    //    tok.raises_keyword
    //    >> '('
    //    >> (scoped_name % ',')
    //    >> ')'
    //   ]
    //   >> qi::attr(true)
    //   ;
    // direction = 
    //   tok.in_keyword[_val = idl_parser::direction::in()]
    //   | tok.out_keyword[_val = idl_parser::direction::out()]
    //   | tok.inout_keyword[_val = idl_parser::direction::inout()]
    //   ;
    // param %=
    //   direction
    //   >> type_spec
    //   >> qi::omit[tok.identifier]
    //   ;      
    // // start.name("op_decl");
    // // param.name("param");
    // // direction.name("direction");
    // // qi::debug(start);
    // // qi::debug(param);
    // // qi::debug(direction);
  }

  grammar::scoped_name<Iterator> scoped_name;
  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, boost::variant<idl_parser::direction::in
                                    , idl_parser::direction::out
                                    , idl_parser::direction::inout>()> direction;
  qi::rule<Iterator, idl_parser::param_decl<Iterator>()> param;
  qi::rule<Iterator, idl_parser::attr_decl<Iterator>()> start;
};

} } }

#endif
