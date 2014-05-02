/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_PARSE_TYPES_HPP
#define MORBID_IDL_COMPILER_PARSE_TYPES_HPP

#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpp_context.hpp>

namespace morbid { namespace idlc {

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator
  <token_type> lex_iterator_type;
typedef boost::wave::context
  <std::vector<char>::const_iterator, lex_iterator_type> context_type;

typedef context_type::iterator_type parser_iterator_type;

} }

#endif // MORBID_IDL_COMPILER_PARSE_TYPES_HPP
