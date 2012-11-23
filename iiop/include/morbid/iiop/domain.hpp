/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_DOMAIN_HPP
#define MORBID_IIOP_DOMAIN_HPP

#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

struct generator_compiler;

struct generator_domain
{
  typedef generator_compiler compiler;
};

struct parser_domain {};

} }

#endif

