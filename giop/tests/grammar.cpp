/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/grammar.hpp>

#include "test_compile.hpp"

struct rule
{
  rule() {}

  template <typename C>
  rule& operator=(C const& c)
  {
    return *this;
  }

  template <typename C>
  rule& operator%=(C const& c)
  {
    return *this;
  }

private:
  rule(rule const&) {}
  rule& operator=(rule const&);
};

namespace morbid { namespace giop {

template <typename Domain, typename I, typename T1, typename T2, typename T3
          , typename T4>
struct rule_impl
{
  typedef ::rule type;
};

} }

struct float_terminal {};
struct rule_terminal {};
struct rule_terminal_params {};

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

template <typename D, typename I, typename T1, typename T2, typename T3, typename T4
          , typename Modifiers, typename Enable>
struct make_primitive<boost::reference_wrapper
                      <morbid::giop::rule<D, I, T1, T2, T3, T4>const>
                      , Modifiers, Enable>
{
  typedef ::rule_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename D1, typename D2, typename I, typename T1, typename T2, typename T3, typename T4
          , typename Params, typename Modifiers, typename Enable>
struct make_primitive<morbid::giop::parameterized_nonterminal
                      <D1, morbid::giop::rule<D2, I, T1, T2, T3, T4>, Params>
                      , Modifiers, Enable>
{
  typedef ::rule_terminal_params result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

// Start of actual test
template <typename Domain, typename Iterator>
struct grammar_with_attrs : morbid::giop::grammar<Domain, Iterator, char(int)>
{
  grammar_with_attrs() : grammar_with_attrs::base_type(start)
  {
  }
  
  morbid::giop::rule<Domain, Iterator, char(int)> start;
};

template <typename Domain, typename Iterator>
struct grammar : morbid::giop::grammar<Domain, Iterator, char()>
{
  grammar() : grammar::base_type(start)
  {
  }
  
  morbid::giop::rule<Domain, Iterator, char()> start;
};

template <typename CExpr>
void test(CExpr const& expr)
{
  std::cout << "CExpr " << typeid(expr).name() << std::endl;
  BOOST_MPL_ASSERT_RELATION(boost::fusion::result_of::size<typename CExpr::elements>::value, ==, 3);
  assert(boost::fusion::result_of::size<typename CExpr::elements>::value == 3);
}

int main()
{
  grammar_with_attrs< test_compile::domain, const char*> g1;
  grammar< test_compile::domain, const char*> g2;
  test(morbid::giop::compile<test_compile::domain>(morbid::giop::float_ & g1(5) & g2));
}

