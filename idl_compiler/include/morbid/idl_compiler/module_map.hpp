/* Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_MODULE_MAP_HPP
#define MORBID_IDL_COMPILER_MODULE_MAP_HPP

#include <morbid/idl_compiler/common_types.hpp>

namespace morbid { namespace idlc {

typedef boost::property_map<morbid::idlc::modules_tree_type, morbid::idlc::module_property_t>::type module_map;

} }

#endif // MORBID_IDL_COMPILER_MODULE_MAP_HPP
