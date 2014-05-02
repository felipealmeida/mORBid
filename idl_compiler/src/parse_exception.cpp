/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/parse_functions.hpp>

#include <morbid/idl_compiler/skipper.hpp>

#include <morbid/idl_parser/grammar/exception_def.hpp>

namespace morbid { namespace idlc {

namespace
{
morbid::idl_parser::grammar::exception_definition<parser_iterator_type> const exception_grammar;
}

bool parse_exception(parser_iterator_type& iterator, parser_iterator_type const& last, morbid::idl_parser::exception_def& exception)
{
  return boost::spirit::qi::phrase_parse
    (iterator, last
    , (exception_grammar[boost::phoenix::ref(exception) = boost::spirit::qi::_1]
    >> morbid::idl_parser::token_value(";"))
    , skipper);
}

} }
