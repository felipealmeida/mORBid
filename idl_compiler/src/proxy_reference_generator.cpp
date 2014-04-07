/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

//#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#if 1 //defined(MORBID_IDLC_DEBGUG)
#include <boost/fusion/include/io.hpp>
#endif

#include <morbid/idl_compiler/generator/proxy_reference_generator.ipp>
#include <morbid/idl_compiler/iterator.hpp>

template struct
morbid::idlc::generator::proxy_reference_generator
 < ::morbid::idlc::output_iterator>;

