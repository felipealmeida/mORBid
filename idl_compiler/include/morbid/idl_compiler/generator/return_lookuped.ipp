/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_RETURN_LOOKUPED_IPP
#define MORBID_IDL_RETURN_LOOKUPED_IPP

#include <morbid/idl_compiler/generator/return_lookuped.hpp>

#include <boost/phoenix.hpp>

namespace morbid { namespace idlc { namespace generator {

namespace karma = boost::spirit::karma;

template <typename OutputIterator>
return_lookuped<OutputIterator>::return_lookuped()
  : return_lookuped::base_type(start)
{
  using karma::_1;
  using karma::_val;
  using karma::_r1;
  using karma::_r2;

  start = "typename ::morbid::lazy_eval< " << return_traits(_r1)[_1 = _val] << ", " << karma::lit(_r2) << ">::type";

  karma::debug(start);
}

} } }

#endif
