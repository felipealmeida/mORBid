/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PARSE_SKIPPER_HPP
#define MORBID_IDL_COMPILER_PARSE_SKIPPER_HPP

#include <morbid/idl_compiler/parse_types.hpp>
#include <morbid/idl_parser/grammar/skipper.hpp>

namespace morbid { namespace idlc {

extern morbid::idl_parser::grammar::skipper<morbid::idlc::parser_iterator_type> skipper;

} }

#endif // MORBID_IDL_COMPILER_PARSE_SKIPPER_HPP
