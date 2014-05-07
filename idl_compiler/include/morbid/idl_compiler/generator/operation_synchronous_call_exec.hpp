/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_OPERATION_SYNCHRONOUS_CALL_EXEC_HPP
#define MORBID_IDL_COMPILER_OPERATION_SYNCHRONOUS_CALL_EXEC_HPP

#include <morbid/idl_compiler/interface.hpp>
#include <morbid/idl_compiler/generator/indent.hpp>
#include <morbid/idl_compiler/generator/type_spec.hpp>

#include <morbid/idl_parser/interface_def.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct operation_synchronous_call_exec : karma::grammar
<OutputIterator, idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
, karma::locals<unsigned int> >
{
  operation_synchronous_call_exec();

  idlc::generator::indent<OutputIterator> indent;
  idlc::generator::type_spec<OutputIterator> type_spec;
  karma::rule<OutputIterator, idl_parser::type_spec(interface_)>
    type_spec_select;
  karma::rule<OutputIterator, idl_parser::direction::in()> in_tag;
  karma::rule<OutputIterator, idl_parser::direction::out()> out_tag;
  karma::rule<OutputIterator, idl_parser::direction::inout()> inout_tag;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_)>
    synchronous_template_args;
  karma::rule<OutputIterator, idl_parser::param_decl(interface_, unsigned int)> synchronous_args;
  karma::rule<OutputIterator
            , idl_parser::op_decl(interface_, std::vector<idl_parser::wave_string>, idl_parser::wave_string)
            , karma::locals<unsigned int> >
    start;
};

} } }

#endif
