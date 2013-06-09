/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_DETAIL_IS_A_HPP
#define MORBID_ORB_DETAIL_IS_A_HPP

#include <boost/mpl/for_each.hpp>

#include <string>

namespace morbid { namespace detail {

template <typename C>
bool is_a(std::string const& type_id);

struct call_is_a
{
  call_is_a(std::string const& type_id, bool& r)
    : type_id(type_id), r(r) {}

  template <typename C>
  void operator()(C) const
  {
    if (!r)
      r = detail::is_a<C>(type_id);
  }

  std::string const& type_id;
  bool& r;
};

template <typename C>
bool is_a(std::string const& type_id)
{
  bool r = C::type_id() == type_id;
  if(!r)
  {
    boost::mpl::for_each<typename C::bases>(call_is_a(type_id, r));
  }
  return r;
}

} }

#endif
