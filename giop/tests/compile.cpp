/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/giop/common_terminals.hpp>

#include <boost/proto/core.hpp>

struct domain {};

template <typename T, typename Modifiers, typename Enable = void>
struct make_primitive;

struct float_terminal {};
struct double_terminal {};
struct long_double_terminal {};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::float_, Modifiers, Enable>
{
  typedef float_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

template <typename Modifiers, typename Enable>
struct make_primitive<boost::spirit::tag::double_, Modifiers, Enable>
{
  typedef double_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

template <typename Modifiers, typename Enable>
struct make_primitive<morbid::giop::tag::long_double, Modifiers, Enable>
{
  typedef long_double_terminal result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const;
};

template <typename Tag, typename Elements
          , typename Modifiers, typename Enable = void>
struct make_composite;

template <typename Elements>
struct sequence
{
  sequence(Elements const& subject)
  {
  }
};

template <typename Elements, typename Modifiers>
struct make_composite<boost::proto::tag::bitwise_and, Elements, Modifiers>
  : boost::spirit::make_nary_composite<Elements, ::sequence>
{};


namespace boost { namespace spirit {

template <typename Enable>
struct use_operator< ::domain, proto::tag::bitwise_and, Enable> : mpl::true_ {};

template <>
struct flatten_tree< ::domain, proto::tag::bitwise_and>
  : mpl::true_ {};

template <typename Enable>
struct use_terminal< ::domain, tag::float_, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< ::domain, tag::double_, Enable> : mpl::true_ {};

template <typename Enable>
struct use_terminal< ::domain, ::morbid::giop::tag::long_double, Enable> : mpl::true_ {};

template <>
struct make_component< ::domain, proto::tag::terminal>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename ::make_primitive<
      typename remove_const<typename Elements::car_type>::type
      , typename remove_reference<Modifiers>::type
      >::result_type type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    typedef typename remove_const<typename Elements::car_type>::type term;
    return ::make_primitive<term, Modifiers>()(elements.car, modifiers);
  }
};

template <typename Tag>
struct make_component< ::domain, Tag>
{
  template <typename Sig>
  struct result;

  template <typename This, typename Elements, typename Modifiers>
  struct result<This(Elements, Modifiers)>
  {
    typedef typename
    make_composite<Tag, Elements
                   , typename remove_reference<Modifiers>::type>::result_type
    type;
  };

  template <typename Elements, typename Modifiers>
  typename result<make_component(Elements, Modifiers)>::type
  operator()(Elements const& elements, Modifiers const& modifiers) const
  {
    return make_composite<Tag, Elements, Modifiers>()(elements, modifiers);
  }
};

} }

template <typename Elements>
void test( ::sequence<Elements> const& s)
{
  BOOST_MPL_ASSERT_RELATION(boost::mpl::size<Elements>::value, ==, 3);
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 0>::type, float_terminal>));
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 1>::type, double_terminal>));
  BOOST_MPL_ASSERT((boost::is_same<typename boost::mpl::at_c<Elements, 2>::type, long_double_terminal>));
}

template <typename Expr>
void compile(Expr const& expr)
{
  typedef typename boost::spirit::result_of::compile< ::domain, Expr>::type result_type;
  result_type r = boost::spirit::compile< ::domain>(expr);
  test(r);
}

int main()
{
  namespace giop = morbid::giop;
  compile(giop::float_ & giop::double_ & giop::long_double);
}

