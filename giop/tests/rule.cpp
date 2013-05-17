/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/rule.hpp>

#include "test_compile.hpp"

struct float_terminal {};
struct float_terminal_int {};
struct ushort_terminal {};
struct rule_terminal {};
struct rule_terminal_params {};

namespace boost { namespace spirit {

template <>
struct use_lazy_terminal< test_compile::domain, tag::float_, 1> : mpl::true_ {};

template <typename Enable>
struct use_terminal< test_compile::domain, terminal_ex<tag::float_, fusion::vector1<int> >, Enable> : mpl::true_ {};

} }

namespace test_compile {

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::float_, Modifiers, Enable>
{
  typedef ::float_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::terminal_ex
  <boost::spirit::tag::float_, boost::fusion::vector1<int> >, Modifiers, Enable>
{
  typedef ::float_terminal_int result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::ushort_, Modifiers, Enable>
{
  typedef ::ushort_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::reference_wrapper<rule const>, Modifiers, Enable>
{
  typedef ::rule_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename D1, typename Params, typename Modifiers, typename Enable>
struct make_primitive<morbid::giop::parameterized_nonterminal<D1, rule, Params>, Modifiers, Enable>
{
  typedef ::rule_terminal_params result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

template <typename CExpr>
void test(CExpr const& expr)
{
  // std::cout << "CExpr " << typeid(expr).name() << std::endl;
  BOOST_MPL_ASSERT_RELATION(boost::fusion::result_of::size<typename CExpr::elements>::value, ==, 3);
  assert(boost::fusion::result_of::size<typename CExpr::elements>::value == 3);
}

int main()
{
  namespace giop = morbid::giop;
  giop::rule<test_compile::domain, const char*, char(int)> r;
  r = giop::float_ & giop::ushort_;
  test(giop::compile<test_compile::domain>(giop::float_(5) & r(5) & r));
}

