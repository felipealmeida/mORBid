/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_PARAMETER_HPP
#define TECORB_IDL_COMPILER_PARAMETER_HPP

#include <tecorb/idl_parser/op_decl.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/phoenix.hpp>

namespace tecorb { namespace idl_compiler {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator>
struct parameter : karma::grammar<OutputIterator, idl_parser::param_decl()>
{
  parameter()
    : parameter::base_type(start)
  {
    using karma::_1;
    using karma::_val;
    using phoenix::at_c;

    start =
       (
        karma::eps(at_c<1>(_val) == "string")
        << -(
             karma::eps(at_c<0>(_val) == "in") << "const "
            )
        << "char*"
       )
      | (
         karma::eps(at_c<1>(_val) == "wstring")
         << -(
              karma::eps(at_c<0>(_val) == "in") << "const "
             )
         << "wchar_t*"
        )
      | (
         karma::eps(at_c<1>(_val) == "boolean") << "bool"
         << -(karma::eps(at_c<0>(_val) != "in")
              << "&")
        )
      | (
         karma::eps(at_c<1>(_val) == "octet")
         << -(
              karma::eps(at_c<0>(_val) == "in") << "const "
             )
         << "unsigned char*"
        )
      | (
         karma::eps(at_c<1>(_val) == "wchar") << "wchar_t"
         << -(karma::eps(at_c<0>(_val) != "in")
              << "&")
        )
      | (
         karma::eps(at_c<1>(_val) == "any")
         << -(karma::eps(at_c<0>(_val) == "in") << "const ")
          << "CORBA::Any_ptr"
        )
      | karma::string[_1 = at_c<1>(_val)]
      ;
  }

  karma::rule<OutputIterator, idl_parser::param_decl()> start;
};

} }

#endif
