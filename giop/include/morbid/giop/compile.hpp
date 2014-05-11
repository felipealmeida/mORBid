/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_COMPILE_HPP
#define MORBID_GIOP_COMPILE_HPP

#include <boost/utility/result_of.hpp>

namespace morbid { namespace giop {

template <typename Domain>
struct compile_impl;

namespace result_of {

template <typename Domain, typename Expr>
struct compile : boost::result_of<compile_impl<Domain>(Expr)>
{
};

}

template <typename Domain, typename Expr>
typename boost::result_of<compile_impl<Domain>(Expr)>::type
compile(Expr const& expr)
{
  compile_impl<Domain> c;
  return c(expr);
}

} }

#endif
