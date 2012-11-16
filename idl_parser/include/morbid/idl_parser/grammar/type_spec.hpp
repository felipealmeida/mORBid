/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_TYPE_SPEC_HPP
#define MORBID_IDL_PARSER_GRAMMAR_TYPE_SPEC_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

#include <morbid/idl_parser/tokenizer.hpp>
#include <morbid/idl_parser/type_spec.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;


template <typename Iterator>
struct scoped_name : boost::spirit::qi::grammar
  <Iterator, types::scoped_name(), skipper<Iterator> >
{
  template <typename TokenDef>
  scoped_name(TokenDef const& tok)
    : scoped_name::base_type(start)
  {
    start %=
      qi::lexeme
      [
       ((qi::omit[qi::lit("::")] >> qi::attr(true))
        | qi::attr(false))
       >> (tok.identifier % qi::lit("::"))
      ]
      ;
    // start.name("scoped_name");
    // qi::debug(start);
  }
  boost::spirit::qi::rule<Iterator, types::scoped_name(), skipper<Iterator> > start;
};


template <typename Iterator>
struct type_spec : boost::spirit::qi::grammar
  <Iterator, idl_parser::type_spec<Iterator>()
   , skipper<Iterator> >
{
  typedef typename Iterator::base_iterator_type base_iterator;

  template <typename TokenDef>
  type_spec(TokenDef const& tok)
    : type_spec::base_type(start)
    , scoped_name(tok)
  {
    using qi::_val;
    using qi::_1;
    namespace phoenix = boost::phoenix;
    using phoenix::at_c;
    using phoenix::construct;

    simple_type_spec =
      tok.float_type[_val = types::floating_point::float_]
      | tok.double_type[_val = types::floating_point::double_]
      | (tok.long_type >> tok.double_type)[_val = types::floating_point::long_double_]
      | (tok.short_type)[_val = types::integer::signed_short_int]
      | (tok.long_type >> tok.long_type)[_val = types::integer::signed_longlong_int]
      | (tok.long_type)[_val = types::integer::signed_long_int]
      | (tok.unsigned_type >> tok.short_type)[_val = types::integer::unsigned_short_int]
      | (tok.unsigned_type >> tok.long_type >> tok.long_type)[_val = types::integer::unsigned_longlong_int]
      | (tok.unsigned_type >> tok.long_type)[_val = types::integer::unsigned_long_int]
      | (tok.char_type)[_val = types::char_()]
      | (tok.wchar_type)[_val = types::wchar_()]
      | (tok.boolean_type)[_val = types::boolean()]
      | (tok.octet_type)[_val = types::octet()]
      | (tok.any_type)[_val = types::any()]
      | (tok.object_type)[_val = types::object()]
      | (tok.value_base_type)[_val = types::value_base()]
      | (tok.void_type)[_val = types::void_()]
      | scoped_name[_val = _1]
      ;
    sequence =
      (qi::omit[tok.sequence_type >> '<'] >> simple_type_spec >> qi::omit['>'])
      [_val = construct<types::sequence<Iterator> >(_1)]
      ;
    start %=
      sequence
      | simple_type_spec
      ;
    // start.name("type_spec");
    // qi::debug(start);
    // sequence.name("sequence");
    // qi::debug(sequence);
    // simple_type_spec.name("simple_type_spec");
    // qi::debug(simple_type_spec);
  }

  grammar::scoped_name<Iterator> scoped_name;
  boost::spirit::qi::rule<Iterator, idl_parser::type_spec<Iterator>()
                          , skipper<Iterator> > start;
  boost::spirit::qi::rule<Iterator, idl_parser::type_spec<Iterator>()
                          , skipper<Iterator> > simple_type_spec;
  boost::spirit::qi::rule<Iterator, types::sequence<Iterator>()
                          , skipper<Iterator> > sequence;
};

} } }

#endif
