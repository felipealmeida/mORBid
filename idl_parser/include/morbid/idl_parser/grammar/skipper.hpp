/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_SKIPPER_HPP
#define MORBID_IDL_PARSER_GRAMMAR_SKIPPER_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/wave/token_ids.hpp>

#include <morbid/idl_parser/token.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

template <typename Iterator>
struct skipper : boost::spirit::qi::grammar<Iterator>
{
  skipper() : skipper::base_type(start)
  {
    start = token_category(boost::wave::WhiteSpaceTokenType)
      | token_category(boost::wave::EOLTokenType)
      | token_category(static_cast<boost::wave::token_category>
                       (boost::wave::WhiteSpaceTokenType|boost::wave::AltTokenType))

      ;
  }

  boost::spirit::qi::rule<Iterator> start;
};

} } }

#endif
