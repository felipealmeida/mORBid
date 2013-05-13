/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_PARSER_GRAMMAR_TYPE_SPEC_HPP
#define MORBID_IDL_PARSER_GRAMMAR_TYPE_SPEC_HPP

#include <morbid/idl_parser/token.hpp>
#include <morbid/idl_parser/type_spec.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_parser { namespace grammar {

namespace qi = boost::spirit::qi;
namespace lex = boost::spirit::lex;


template <typename Iterator>
struct scoped_name : boost::spirit::qi::grammar
  <Iterator, types::scoped_name(), skipper<Iterator> >
{
  scoped_name()
    : scoped_name::base_type(start)
  {
    start %=
       ((qi::omit[token_id(boost::wave::T_COLON_COLON)] >> qi::attr(true))
        | qi::attr(false))
       >> ((&token_id(boost::wave::T_IDENTIFIER) >> token_value)
           % token_id(boost::wave::T_COLON_COLON))
      ;
    // start.name("scoped_name");
    // qi::debug(start);
  }
  boost::spirit::qi::rule<Iterator, types::scoped_name(), skipper<Iterator> > start;
};


template <typename Iterator>
struct type_spec : boost::spirit::qi::grammar
 <Iterator, idl_parser::type_spec(), skipper<Iterator> >
{
  type_spec()
    : type_spec::base_type(start)
  {
    using qi::_val;
    using qi::_1;
    namespace phoenix = boost::phoenix;
    using phoenix::at_c;
    using phoenix::construct;

    simple_type_spec =
      token_id(boost::wave::T_FLOAT)[_val = types::floating_point::float_]
      | token_id(boost::wave::T_DOUBLE)[_val = types::floating_point::double_]
      | (token_id(boost::wave::T_LONG) >> token_id(boost::wave::T_DOUBLE))[_val = types::floating_point::long_double_]
      | token_id(boost::wave::T_SHORT)[_val = types::integer::signed_short_int]
      | (token_id(boost::wave::T_LONG) >> token_id(boost::wave::T_LONG))[_val = types::integer::signed_longlong_int]
      | token_id(boost::wave::T_LONG)[_val = types::integer::signed_long_int]
      | (token_id(boost::wave::T_UNSIGNED) >> token_id(boost::wave::T_SHORT))[_val = types::integer::unsigned_short_int]
      | (token_id(boost::wave::T_UNSIGNED) >> token_id(boost::wave::T_LONG) >> token_id(boost::wave::T_LONG))
        [_val = types::integer::unsigned_longlong_int]
      | (token_id(boost::wave::T_UNSIGNED) >> token_id(boost::wave::T_LONG))[_val = types::integer::unsigned_long_int]
      | token_id(boost::wave::T_CHAR)[_val = types::char_()]
      | token_id(boost::wave::T_WCHART)[_val = types::wchar_()]
      | token_id(boost::wave::T_BOOL)[_val = types::boolean()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("octet"))[_val = types::octet()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("any"))[_val = types::any()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("object"))[_val = types::object()]
      | (&token_id(boost::wave::T_IDENTIFIER) >> token_value("ValueBase"))[_val = types::value_base()]
      | token_id(boost::wave::T_VOID)[_val = types::void_()]
      | scoped_name[_val = _1]
      ;
    sequence =
      (qi::omit[&token_id(boost::wave::T_IDENTIFIER) >> token_value("sequence") >> '<']
       >> simple_type_spec >> qi::omit['>'])
      [_val = construct<types::sequence>(_1)]
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
  boost::spirit::qi::rule<Iterator, idl_parser::type_spec(), skipper<Iterator> > start;
  boost::spirit::qi::rule<Iterator, idl_parser::type_spec(), skipper<Iterator> > simple_type_spec;
  boost::spirit::qi::rule<Iterator, types::sequence(), skipper<Iterator> > sequence;
};

} } }

#endif
