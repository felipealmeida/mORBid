/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_DETAIL_REQUIREMENTS_HPP
#define MORBID_ORB_DETAIL_REQUIREMENTS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/fold.hpp>

namespace morbid { namespace detail {

namespace mpl = boost::mpl;

template <typename C>
struct requirements
{
  typedef typename C::requirements derived_requirements;
  typedef typename mpl::fold
  <typename C::bases
   , derived_requirements
   , mpl::joint_view<mpl::_1, requirements<mpl::_2> >
   >::type type;
};

template <typename C>
struct regular_requirements
{
  typedef typename C::regular_requirements derived_requirements;
  typedef typename mpl::fold
  <typename C::bases
   , derived_requirements
   , mpl::joint_view<mpl::_1, requirements<mpl::_2> >
   >::type type;
};

} }

#endif
