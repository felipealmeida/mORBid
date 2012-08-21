/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_IDL_COMPILER_COMMON_TYPES
#define TECORB_IDL_COMPILER_COMMON_TYPES

#include <boost/spirit/home/lex.hpp>
#include <boost/spirit/home/lex/lexer_lexertl.hpp>

#include <vector>
#include <iterator>
#include <ostream>

namespace tecorb { namespace idl_compiler {

typedef boost::spirit::lex::lexertl::token<std::vector<char>::const_iterator> token_type;
typedef boost::spirit::lex::lexertl::actor_lexer<token_type> lexer_type;
        
typedef lexer_type::iterator_type parser_iterator_type;
typedef tecorb::idl_parser::interface_def<parser_iterator_type> interface_def_type;
typedef std::ostream_iterator<char> output_iterator_type;


} }

#endif
