/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_STRUCT_DEF_HPP
#define MORBID_IDL_PARSER_GRAMMAR_STRUCT_DEF_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/grammar/type_spec.hpp>
#include <morbid/idl_parser/grammar/member.hpp>
#include <morbid/idl_parser/struct_def.hpp>

namespace std {

inline std::ostream& operator<<(std::ostream& os
                         , std::vector< ::morbid::idl_parser::member>const& v)
{
  os << "[vector ";
  std::copy(v.begin(), v.end()
            , std::ostream_iterator< ::morbid::idl_parser::member>
            (os, ", "));
  return os << "]";
}

}

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct struct_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::struct_def()
   , qi::locals<std::vector<member> >
   , skipper<Iterator> >
{
  struct_definition()
    : struct_definition::base_type(start)
  {
    namespace p = boost::phoenix;
    using qi::_a; using qi::_b;
    using qi::_1;
    using p::at_c;
    using qi::_val;

    start =
      &token_position[at_c<2>(_val) = _1]
      >> token_id(boost::wave::T_STRUCT)
      >> &token_id(boost::wave::T_IDENTIFIER)
      >> token_value[at_c<0>(_val) = _1]
      >> token_id(boost::wave::T_LEFTBRACE)
      >> *(members[p::insert(_a, p::end(_a), p::begin(_1), p::end(_1))])
      >> token_id(boost::wave::T_RIGHTBRACE)
      >> qi::eps[at_c<1>(_val) = _a]
      ;
    start.name("struct_def");
    qi::debug(start);
  }

  grammar::type_spec<Iterator> type_spec;
  grammar::member_definition<Iterator> members;
  qi::rule<Iterator, idl_parser::struct_def()
           , qi::locals<std::vector<member> >, skipper<Iterator> > start;
};

} } }

#endif
