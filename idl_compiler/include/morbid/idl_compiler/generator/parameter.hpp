/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_PARAMETER_HPP
#define TECORB_IDL_COMPILER_PARAMETER_HPP

#include <morbid/idl_parser/op_decl.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace morbid { namespace idl_compiler { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename Iterator>
struct parameter : karma::grammar<OutputIterator, idl_parser::param_decl<Iterator>()>
{
  parameter()
    : parameter::base_type(start)
  {
    using karma::_1;
    using karma::_val;
    using phoenix::at_c;

    start = param(at_c<0>(_val))[_1 = at_c<1>(_val)];
    scoped_name %=
      (
       karma::true_ << "::"
       | karma::eps
      )
      << (+karma::string)
      ;
    param = 
      (floating_point | integer | char_ | wchar_ | boolean | octet
       | any | object | value_base | void_ | scoped_name
       | sequence) [_1 = at_c<0>(_val)]
      ;
    start.name("parameter");
    karma::debug(start);
    scoped_name.name("scoped_name");
    karma::debug(scoped_name);
    param.name("param");
    karma::debug(param);

    //    (
    //     karma::eps(at_c<1>(_val) == "string")
    //     << -(
    //          karma::eps(at_c<0>(_val) == "in") << "const "
    //         )
    //     << "char*"
    //    )
    //   | (
    //      karma::eps(at_c<1>(_val) == "wstring")
    //      << -(
    //           karma::eps(at_c<0>(_val) == "in") << "const "
    //          )
    //      << "wchar_t*"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "boolean") << "CORBA::Boolean"
    //      << -(karma::eps(at_c<0>(_val) != "in")
    //           << "&")
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "octet")
    //      << -(
    //           karma::eps(at_c<0>(_val) == "in") << "const "
    //          )
    //      << "unsigned char*"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "wchar") << "wchar_t"
    //      << -(karma::eps(at_c<0>(_val) != "in")
    //           << "&")
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "any")
    //      << -(karma::eps(at_c<0>(_val) == "in") << "const ")
    //       << "CORBA::Any_ptr"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "long")
    //      << "CORBA::Long"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "short")
    //      << "CORBA::Short"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "double")
    //      << "CORBA::Double"
    //     )
    //   | (
    //      karma::eps(at_c<1>(_val) == "float")
    //      << "CORBA::Float"
    //     )
    //   | karma::string[_1 = at_c<1>(_val)]
    //   ;
  }

  karma::rule<OutputIterator, idl_parser::param_decl<Iterator>()> start;
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
  karma::rule<OutputIterator, idl_parser::type_spec<Iterator>
              (boost::variant<idl_parser::direction::in
               , idl_parser::direction::out, idl_parser::direction::inout>)>
  param;
  
};


} } }

#endif
