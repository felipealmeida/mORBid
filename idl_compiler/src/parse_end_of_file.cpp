/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_parser/token.hpp>

namespace morbid { namespace idlc {

bool parse_end_of_file(parser_iterator_type& iterator, parser_iterator_type const& last)
{
  using morbid::idl_parser::token_id;
  return boost::spirit::qi::parse(iterator, last, token_id(boost::wave::T_EOI) | token_id(boost::wave::T_EOF));
}

} }
