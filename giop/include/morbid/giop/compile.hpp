/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_COMPILE_HPP
#define MORBID_GIOP_COMPILE_HPP

namespace morbid { namespace giop {

template <typename Domain, typename Expr>
typename boost::spirit::result_of::compile< Domain, Expr>::type
compile(Expr const& expr)
{
  typedef typename boost::spirit::result_of::compile< Domain, Expr>::type result_type;
  std::cout << "expr " << typeid(result_type).name() << std::endl;
  return boost::spirit::compile< Domain>(expr);
}

} }

#endif
