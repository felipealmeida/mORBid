/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generator/typedef_generator.ipp>
#include <morbid/idl_compiler/iterator.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave.hpp>

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator
<token_type> lex_iterator_type;
typedef boost::wave::context
<std::vector<char>::const_iterator, lex_iterator_type> context_type;

typedef context_type::iterator_type parser_iterator_type;

template struct
morbid::idlc::generator::typedef_generator
< ::morbid::idlc::output_iterator, parser_iterator_type>;

