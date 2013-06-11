/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_RULE_HPP
#define MORBID_GIOP_RULE_HPP

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/compile.hpp>
#include <morbid/giop/detail/parameterized_nonterminal.hpp>
#include <morbid/giop/detail/max_args.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace morbid { namespace giop {

struct little_endian_type {};
struct big_endian_type {};
struct native_endian_type {};

little_endian_type const little_endian = {};
big_endian_type const big_endian = {};
native_endian_type const native_endian = {};

struct endian
{
  endian(bool is_little_endian) : b(is_little_endian) {}
  endian(little_endian_type) : b(true) {}
  endian(big_endian_type) : b(false) {}
  endian(native_endian_type)
#ifdef BOOST_LITTLE_ENDIAN
    : b(true)
#else
    : b(false)
#endif
  {}

  bool b;
};

} }

namespace boost {

template <>
struct is_scalar< ::morbid::giop::little_endian_type> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::big_endian_type> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::native_endian_type> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::endian> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::little_endian_type const> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::big_endian_type const> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::native_endian_type const> : mpl::true_ {};
template <>
struct is_scalar< ::morbid::giop::endian const> : mpl::true_ {};

}

namespace morbid { namespace giop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;

template <typename Domain, typename I, typename T1, typename T2, typename T3
          , typename T4>
struct rule_impl;

template <typename T>
struct auto_expr_type
{
  typedef T type;

  T const& expr;

  auto_expr_type(T const& expr)
    : expr(expr) {}
};

template <typename T>
auto_expr_type<T> auto_expr(T const& expr)
{
  return auto_expr_type<T>(expr);
}

template <typename Domain, typename Iterator, typename T1, typename T2 = spirit::unused_type
          , typename T3 = spirit::unused_type, typename T4 = spirit::unused_type>
struct rule
  : proto::extends
    <typename proto::terminal
     <boost::reference_wrapper
      <typename rule_impl<Domain, Iterator, T1, T2, T3, T4>::type const> >::type
     , rule<Domain, Iterator, T1, T2, T3, T4>
    >
{
  typedef rule<Domain, Iterator, T1, T2, T3, T4> self_type;
  typedef typename giop::rule_impl<Domain, Iterator, T1, T2, T3, T4>::type rule_impl;
  typedef typename proto::terminal<boost::reference_wrapper<rule_impl const> >::type terminal_type;

  static size_t const params_min_size = rule_impl::params_min_size;
  static size_t const params_max_size = rule_impl::params_max_size;

  rule()
    : proto::extends<terminal_type, self_type>(terminal_type::make(boost::cref(rule_impl_)))
  {}
  rule(const char* name)
    : proto::extends<terminal_type, self_type>(terminal_type::make(boost::cref(rule_impl_)))
    , rule_impl_(name)
  {}
  ~rule()
  {
    // std::cout << "~rule " << this << std::endl;
  }

  template <typename Expr>
  self_type& operator=(Expr const& expr)
  {
    rule_impl_ = expr;
    return *this;
  }

  template <typename Expr>
  self_type& operator=(auto_expr_type<Expr> const& expr)
  {
    rule_impl_ = expr;
    return *this;
  }

  typedef Domain domain_type;

  rule_impl const& get_parameterized_subject() const { return rule_impl_; }
  typedef rule_impl parameterized_subject_type;
#if 0
#include <morbid/giop/detail/nonterminal_fcall.hpp>
#endif
#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, MORBID_GIOP_MAX_ARGS, "morbid/giop/detail/nonterminal_fcall.hpp"))
#include BOOST_PP_ITERATE()

  rule_impl rule_impl_;
private:
  rule(self_type const&);
  self_type& operator=(self_type const&);
};

template <typename Domain, typename Iterator, typename T1, typename T2
          , typename T3, typename T4>
void debug(rule<Domain, Iterator, T1, T2, T3, T4>& r)
{
  debug(r.rule_impl_);
}

} }

#endif
