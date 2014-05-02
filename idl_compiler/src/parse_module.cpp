/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_compiler/skipper.hpp>

#include <morbid/idl_parser/grammar/module_open.hpp>

namespace morbid { namespace idlc {

namespace
{
morbid::idl_parser::grammar::module_open<parser_iterator_type> const module_open_grammar;
}

bool parse_module(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::wave_string& module_open)
{
  return boost::spirit::qi::phrase_parse(iterator, last, module_open_grammar, skipper, module_open);
}

} }

