/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/generator/stub_generator.ipp>
#include <morbid/idl_compiler/common_types.hpp>

template struct
morbid::idl_compiler::generator::header_stub_generator
 < ::morbid::idl_compiler::output_iterator_type
   , ::morbid::idl_compiler::parser_iterator_type>;
// template struct
// morbid::idl_compiler::generator::cpp_stub_generator
//  < ::morbid::idl_compiler::output_iterator_type
//    , ::morbid::idl_compiler::parser_iterator_type>;

