/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/compile.hpp>

#include <boost/proto/core.hpp>

#include "test_compile.hpp"

struct float_terminal {};
struct double_terminal {};
struct long_double_terminal {};

namespace test_compile {

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::float_, Modifiers, Enable>
{
  typedef ::float_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::double_, Modifiers, Enable>
{
  typedef ::double_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

template <typename Modifiers, typename Enable>
struct make_primitive<morbid::giop::tag::long_double, Modifiers, Enable>
{
  typedef ::long_double_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

}

template <typename Elements>
void test( test_compile::sequence<Elements> const& s)
{
  BOOST_MPL_ASSERT_RELATION(boost::mpl::size<Elements>::value, ==, 3);
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 0>::type, float_terminal>));
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 1>::type, double_terminal>));
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 2>::type, long_double_terminal>));
}

template <typename Expr>
void compile(Expr const& expr)
{
  typedef typename morbid::giop::result_of::compile< test_compile::domain, Expr>::type result_type;
  result_type r = morbid::giop::compile< test_compile::domain>(expr);
  test(r);
}

int main()
{
  namespace giop = morbid::giop;
  compile(giop::float_ & giop::double_ & giop::long_double);
}

