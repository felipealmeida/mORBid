/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_RULE_HPP
#define MORBID_GIOP_RULE_HPP

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/compile.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace giop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;
namespace mpl = boost::mpl;

template <typename Domain, typename Iterator, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void>
struct rule
{
  typedef rule<Domain, Iterator, T1, T2, T3, T4> self_type;

  static size_t const params_size = 1u;
    // fusion::result_of::size<parameter_types>::type::value;

  rule() {}
  ~rule()
  {
    std::cout << "~rule " << this << std::endl;
  }

  template <typename Expr>
  void define(Expr const& expr, mpl::false_)
  {
    // Report invalid expression error as early as possible.
    // If you got an error_invalid_expression error message here,
    // then the expression (expr) is not a valid spirit karma expression.
    BOOST_SPIRIT_ASSERT_MATCH(karma::domain, Expr);
  }

  template <typename Expr>
  self_type& operator=(Expr const& expr)
  {
    std::cout << "rule operator= this: " << this << std::endl;
    typedef typename giop::result_of::compile<Domain, Expr>::type compilation_result;
    std::cout << "compilation result " << typeid(compilation_result).name() << std::endl;
    BOOST_MPL_ASSERT((spirit::traits::is_generator<compilation_result>)); 
    f = karma::detail::bind_generator<mpl::false_>(giop::compile<Domain>(expr));
    // assert(!!rule_.f);
    return *this;
  }

  template <typename Expr>
  self_type& operator%=(Expr const& expr)
  {
    std::cout << "rule operator%= this: " << this << std::endl;
    typedef typename giop::result_of::compile<Domain, Expr>::type compilation_result;
    std::cout << "compilation result " << typeid(compilation_result).name() << std::endl;
    BOOST_MPL_ASSERT((spirit::traits::is_generator<compilation_result>)); 
    f = karma::detail::bind_generator<mpl::true_>(giop::compile<Domain>(expr));
    // assert(!!rule_.f);
    return *this;
  }

  typedef typename Domain::template rule<Iterator, T1, T2, T3, T4>::type rule_type;
  rule_type rule_;
  typedef boost::function<
    bool(typename rule_type::output_iterator&, typename rule_type::context_type&
         , typename rule_type::delimiter_type const&)>
  function_type;
  function_type f;
private:
  rule(self_type const&);
  self_type& operator=(self_type const&);
};

} }

#endif
