/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_COMPILER_HPP
#define MORBID_IIOP_COMPILER_HPP

#include <morbid/iiop/domain.hpp>

#include <morbid/iiop/alignment.hpp>

namespace morbid { namespace giop {

namespace spirit = boost::spirit;

template <>
struct compile_impl< iiop::generator_domain>
{
  template <typename T>
  struct result;
  template <typename This, typename Expr>
  struct result<This(Expr)>
  {
    typedef typename boost::proto::subscript<spirit::terminal< iiop::tag::aligned>
                                             , Expr>::type new_expr;
    typedef typename spirit::result_of::compile< iiop::generator_domain, new_expr>::type type;
  };

  template <typename Expr>
  typename result<compile_impl< iiop::generator_domain>(Expr)>::type
  operator()(Expr const& expr) const
  {
    return spirit::compile< iiop::generator_domain>(iiop::aligned[expr]);
  }
};

template <>
struct compile_impl< iiop::parser_domain>
{
  template <typename T>
  struct result;
  template <typename This, typename Expr>
  struct result<This(Expr)>
  {
    // typedef typename boost::proto::subscript<spirit::terminal< iiop::tag::aligned>
    //                                          , Expr>::type new_expr;
    typedef typename spirit::result_of::compile< iiop::parser_domain, Expr>::type type;
  };

  template <typename Expr>
  typename result<compile_impl< iiop::parser_domain>(Expr)>::type
  operator()(Expr const& expr) const
  {
    return spirit::compile< iiop::parser_domain>(expr);
  }
};

} }

#endif
