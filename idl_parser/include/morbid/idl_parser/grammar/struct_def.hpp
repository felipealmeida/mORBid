/* (c) Copyright 2012 Felipe Magno de Almeida
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
#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/struct_def.hpp>

namespace std {

template <typename Iterator>
std::ostream& operator<<(std::ostream& os
                         , std::vector< ::morbid::idl_parser::struct_member<Iterator> >const& v)
{
  os << "[vector ";
  std::copy(v.begin(), v.end()
            , std::ostream_iterator< ::morbid::idl_parser::struct_member<Iterator> >
            (os, ", "));
  return os << "]";
}

}

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;

template <typename Iterator>
struct struct_definition : boost::spirit::qi::grammar
  <Iterator, idl_parser::struct_def<Iterator>()
   , qi::locals<std::vector<struct_member<Iterator> > > >
{
  struct_definition()
    : struct_definition::base_type(start)
  {
    // namespace p = boost::phoenix;
    // using qi::_a; using qi::_b;
    // using qi::_1;
    // using p::at_c;
    // using qi::_val;

    // members =
    //   type_spec[_a = _1]
    //   >> (
    //       tok.identifier
    //       [
    //        p::push_back(_b, p::construct<struct_member<Iterator> >(_a, _1))
    //       ]
    //       % ','
    //      )
    //   >> qi::char_(';')[_val = _b]
    //   ;
    // start =
    //   tok.struct_keyword
    //   >> tok.identifier[at_c<0>(_val) = _1]
    //   >> qi::char_('{')
    //   >> *(members[p::insert(_a, p::end(_a), p::begin(_1), p::end(_1))])
    //   >> qi::char_('}')
    //   >> qi::eps[at_c<1>(_val) = _a]
    //   ;
    // start.name("struct_def");
    // qi::debug(start);
    // members.name("members");
    // qi::debug(members);
  }

  grammar::type_spec<Iterator> type_spec;
  qi::rule<Iterator, std::vector<struct_member<Iterator> >()
           , qi::locals<idl_parser::type_spec<Iterator>
                        , std::vector<struct_member<Iterator> > > > members;
  qi::rule<Iterator, idl_parser::struct_def<Iterator>()
           , qi::locals<std::vector<struct_member<Iterator> > > > start;
};

} } }

#endif
