/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <tecorb/idl_compiler/generator/poa_stub_generator.ipp>
#include <tecorb/idl_compiler/common_types.hpp>

template struct
tecorb::idl_compiler::generator::header_poa_stub_generator
 < ::tecorb::idl_compiler::output_iterator_type
   , ::tecorb::idl_compiler::parser_iterator_type>;
template struct
tecorb::idl_compiler::generator::cpp_poa_stub_generator
 < ::tecorb::idl_compiler::output_iterator_type
   , ::tecorb::idl_compiler::parser_iterator_type>;

