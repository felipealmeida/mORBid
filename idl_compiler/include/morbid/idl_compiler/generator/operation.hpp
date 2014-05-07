/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_OPERATION_HPP
#define MORBID_IDL_COMPILER_OPERATION_HPP

#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/generator/parameter.hpp>
#include <morbid/idl_compiler/generator/return_lookuped.hpp>
#include <morbid/idl_compiler/generator/indent.hpp>
#include <morbid/idl_compiler/generator/operation_synchronous_call_exec.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct operation : karma::grammar
<OutputIterator, idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
, karma::locals<unsigned int> >
{
  operation();

  idlc::generator::parameter<OutputIterator> parameter;
  idlc::generator::return_lookuped<OutputIterator> return_;
  idlc::generator::indent<OutputIterator> indent;
  idlc::generator::operation_synchronous_call_exec<OutputIterator> operation_synchronous_call_exec;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    parameter_select;
  karma::rule<OutputIterator
            , idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
            , karma::locals<unsigned int> >
    start;
};

} } }

#endif
