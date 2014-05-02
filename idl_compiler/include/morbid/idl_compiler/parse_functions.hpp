/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PARSE_FUNCTIONS_HPP
#define MORBID_IDL_COMPILER_PARSE_FUNCTIONS_HPP

#include <morbid/idl_compiler/parse_types.hpp>

#include <morbid/idl_parser/constant.hpp>
#include <morbid/idl_parser/exception_def.hpp>
#include <morbid/idl_parser/interface_def.hpp>
#include <morbid/idl_parser/struct_def.hpp>
#include <morbid/idl_parser/typedef_def.hpp>

namespace morbid { namespace idlc {

bool parse_constant(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::constant& constant);

bool parse_end_of_file(parser_iterator_type& iterator, parser_iterator_type const& last);

bool parse_exception(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::exception_def& exception);

bool parse_interface(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::interface_def& interface);

bool parse_module(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::wave_string& module_open);

bool parse_module_close(parser_iterator_type& iterator, parser_iterator_type const& last);

bool parse_struct(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::struct_def& struct_);

bool parse_typedef(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::typedef_def& typedef_);

} }

#endif // MORBID_IDL_COMPILER_PARSE_FUNCTIONS_HPP
