/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_ATTRIBUTE_HPP
#define MORBID_IDL_PARSER_GRAMMAR_ATTRIBUTE_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/phoenix/fusion/at.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>
#include <morbid/idl_parser/attribute.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct attribute_definition
  : qi::grammar<Iterator, attribute()
                , qi::locals<boost::wave::util::file_position_type>
                , skipper<Iterator> >
{
  attribute_definition() : attribute_definition::base_type(start)
  {
    using qi::_a;
    using qi::_1;
    namespace p = boost::phoenix;

    start %=
      &(token_position[_a = _1])
      >> (
          (&token_id(boost::wave::T_IDENTIFIER) >> token_value("readonly") >> qi::attr(true))
          | qi::attr(false)
         )
      >> (&token_id(boost::wave::T_IDENTIFIER) >> token_value("attribute"))
      >> type_spec
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value
      ;
  }

  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, attribute()
           , qi::locals<boost::wave::util::file_position_type>
           , skipper<Iterator> > start;
};

} } }


#endif

