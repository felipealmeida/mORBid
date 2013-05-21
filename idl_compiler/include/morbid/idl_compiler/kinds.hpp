/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_KINDS_HPP
#define MORBID_IDL_COMPILER_KINDS_HPP

#include <morbid/idl_compiler/common_types.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

namespace morbid { namespace idl_compiler {

struct is_interface_kind {};
struct is_struct_kind {};
struct is_primitive_kind {};
struct is_typedef_kind {};

typedef boost::variant<is_interface_kind, is_struct_kind, is_primitive_kind
                       , is_typedef_kind> kind_variant;

} }

BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_interface_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_struct_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_primitive_kind
                           , BOOST_PP_EMPTY())
BOOST_FUSION_ADAPT_STRUCT( ::morbid::idl_compiler::is_typedef_kind
                           , BOOST_PP_EMPTY())

#endif
