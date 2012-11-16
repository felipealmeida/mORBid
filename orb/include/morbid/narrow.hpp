/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef MORBID_NARROW_HPP
#define MORBID_NARROW_HPP

#include <morbid/object.hpp>
#include <morbid/detail/max_args.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

namespace morbid {

template <typename T, typename Bases, int size
          = boost::mpl::size<Bases>::type::value>
struct narrow;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, MORBID_MAX_ARGS \
                                         , "morbid/narrow.hpp"))
#include BOOST_PP_ITERATE()

}

#endif

#else

#define MORBID_NARROW_base(z, n, data) \
  BOOST_PP_COMMA_IF(n) boost::mpl::at_c<V, n>::type 

template <typename T, typename V>
struct narrow<T, V, BOOST_PP_ITERATION()>
#if BOOST_PP_ITERATION()
  : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), MORBID_NARROW_base, ~)
#endif
{
  static boost::shared_ptr<T> _narrow(boost::shared_ptr<Object> p)
  {
    if(boost::shared_ptr<T> q = boost::dynamic_pointer_cast<T>(p))
      return q;
    else
    {
      if(p->_is_a(T::_repository_id))
        return boost::shared_ptr<T>
          (T::_construct_remote_stub(p->_host(), p->_port()
                                     , p->_objectkey()));
    }
    return boost::shared_ptr<T>();
  }
};

#undef MORBID_NARROW_base

#endif
