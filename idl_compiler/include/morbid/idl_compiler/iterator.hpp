/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_ITERATORS_HPP
#define MORBID_IDL_COMPILER_ITERATORS_HPP

#include <ostream>
#include <iterator>

namespace morbid { namespace idlc {

typedef std::ostream_iterator<char> output_iterator;

} }

#endif
