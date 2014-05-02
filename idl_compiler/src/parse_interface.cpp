/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_compiler/skipper.hpp>

#include <morbid/idl_parser/grammar/interface_def.hpp>
#include <morbid/idl_parser/grammar/interface_forward_def.hpp>
#include <morbid/idl_parser/token.hpp>

namespace morbid { namespace idlc {

namespace
{
morbid::idl_parser::grammar::interface_definition<parser_iterator_type> const interface_grammar;
morbid::idl_parser::grammar::interface_forward_definition<parser_iterator_type> const interface_forward_grammar;
}

bool parse_interface(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::interface_def& interface)
{
  using morbid::idl_parser::token_id;
  return boost::spirit::qi::phrase_parse(iterator, last
                                                 , (interface_grammar | interface_forward_grammar)
                                                 >> token_id(boost::wave::T_SEMICOLON)
                                                 , skipper, interface);
}

} }
