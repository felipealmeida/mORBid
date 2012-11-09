/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_TYPE_SPEC_HPP
#define TECORB_IDL_COMPILER_TYPE_SPEC_HPP

#include <morbid/idl_parser/type_spec.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
struct type_spec : karma::grammar<OutputIterator, idl_parser::type_spec<Iterator>()>
{
  type_spec()
    : type_spec::base_type(start)
  {
    using karma::_1;
    using karma::_val;
    using phoenix::at_c;

    start = 
      (floating_point | integer | char_ | wchar_ | boolean | octet
       | any | object | value_base | void_ | scoped_name
       | sequence) [_1 = at_c<0>(_val)]
      ;
    void_ = karma::string[_1 = "void"];
    scoped_name %=
      (
       karma::true_ << "::"
       | karma::eps
      )
      << (+karma::string)
      ;
  }

  karma::rule<OutputIterator, idl_parser::type_spec<Iterator>()> start;
  karma::rule<OutputIterator, idl_parser::types::floating_point()> floating_point;
  karma::rule<OutputIterator, idl_parser::types::integer()> integer;
  karma::rule<OutputIterator, idl_parser::types::char_()> char_;
  karma::rule<OutputIterator, idl_parser::types::wchar_()> wchar_;
  karma::rule<OutputIterator, idl_parser::types::boolean()> boolean;
  karma::rule<OutputIterator, idl_parser::types::octet()> octet;
  karma::rule<OutputIterator, idl_parser::types::any()> any;
  karma::rule<OutputIterator, idl_parser::types::object()> object;
  karma::rule<OutputIterator, idl_parser::types::value_base()> value_base;
  karma::rule<OutputIterator, idl_parser::types::void_()> void_;
  karma::rule<OutputIterator, idl_parser::types::scoped_name()> scoped_name;
  karma::rule<OutputIterator, idl_parser::types::sequence<Iterator>()> sequence;
  
};

} } }

#endif
