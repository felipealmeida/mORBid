/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>
#include <list>

#include <boost/wave.hpp>
#include <boost/wave/cpp_context.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/wave/grammars/cpp_grammar.hpp>
#include <boost/wave/grammars/cpp_defined_grammar.hpp>
#include <boost/wave/grammars/cpp_expression_grammar.hpp>
#include <boost/wave/cpplexer/re2clex/cpp_re2c_lexer.hpp>
#include <boost/wave/cpplexer/re2clex/cpp_re.hpp>
#include <boost/wave/cpplexer/re2clex/aq.hpp>
#include <boost/wave/cpplexer/re2clex/scanner.hpp>

typedef boost::wave::cpplexer::lex_iterator
  <boost::wave::cpplexer::lex_token<> > lex_iterator_type;
typedef boost::wave::context
  <std::vector<char>::const_iterator, lex_iterator_type> context_type;

typedef context_type::iterator_type parser_iterator_type;

template struct boost::wave::grammars::cpp_grammar_gen
  <lex_iterator_type, context_type::token_sequence_type>;

typedef boost::wave::util::file_position_type position_type;
typedef boost::wave::cpplexer::lex_token<position_type> token_type;
typedef boost::wave::cpplexer::lex_input_interface<token_type> lex_input_interface;

template struct boost::wave::cpplexer::new_lexer_gen
  <
    std::vector<char>::const_iterator, position_type
  , token_type
  >;
template struct boost::wave::cpplexer::new_lexer_gen
  <
    std::vector<char>::iterator, position_type
  , token_type
  >;
template struct boost::wave::cpplexer::new_lexer_gen
  <
    std::string::const_iterator, position_type
  , token_type
  >;
template struct boost::wave::cpplexer::new_lexer_gen
  <
    std::string::iterator, position_type
  , token_type
  >;
template struct boost::wave::grammars::expression_grammar_gen<token_type>;

template struct boost::wave::grammars::defined_grammar_gen<lex_iterator_type>;
