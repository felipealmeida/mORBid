/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ANY_HPP
#define MORBID_ANY_HPP

#include <morbid/var.hpp>

#include <boost/shared_ptr.hpp>

namespace morbid {

struct Any
{
};

typedef boost::shared_ptr<Any> Any_ptr;
typedef morbid::var<Any> Any_var;

}

#endif
