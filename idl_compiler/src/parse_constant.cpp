/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_compiler/skipper.hpp>

#include <morbid/idl_parser/token.hpp>
#include <morbid/idl_parser/grammar/constant.hpp>

namespace morbid { namespace idlc {

namespace
{
morbid::idl_parser::grammar::constant_definition<parser_iterator_type> const constant_grammar;
}

bool parse_constant(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::constant& constant)
{
  using morbid::idl_parser::token_id;
  return boost::spirit::qi::phrase_parse(iterator, last
                                         , constant_grammar >> token_id(boost::wave::T_SEMICOLON)
                                         , skipper, constant);
}

} }