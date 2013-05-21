/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_GRAMMAR_HPP
#define MORBID_GIOP_GRAMMAR_HPP

#include <morbid/giop/common_terminals.hpp>
#include <morbid/giop/rule.hpp>
#include <morbid/giop/compile.hpp>
#include <morbid/giop/detail/parameterized_nonterminal.hpp>
#include <morbid/giop/detail/max_args.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

namespace morbid { namespace giop {

namespace spirit = boost::spirit;
namespace proto = boost::proto;
namespace fusion = boost::fusion;

template <typename Domain, typename Iterator, typename T1, typename T2 = spirit::unused_type
          , typename T3 = spirit::unused_type, typename T4 = spirit::unused_type>
struct grammar
  : proto::extends
  <
    typename proto::terminal
    <boost::reference_wrapper
     <typename rule<Domain, Iterator, T1, T2, T3, T4>::rule_impl const> >::type
    , grammar<Domain, Iterator, T1, T2, T3, T4>
  >
{
  typedef rule<Domain, Iterator, T1, T2, T3, T4> start_type;
  typedef typename rule<Domain, Iterator, T1, T2, T3, T4>::rule_impl rule_impl_type;
  typedef typename proto::terminal
    <boost::reference_wrapper<rule_impl_type const> >::type
    terminal_type;
  typedef grammar<Domain, Iterator, T1, T2, T3, T4> self_type;
  typedef self_type base_type;

  static size_t const params_min_size = start_type::params_min_size;
  static size_t const params_max_size = start_type::params_max_size;

  grammar(start_type const& rule)
    : proto::extends<terminal_type, self_type>(terminal_type::make(boost::cref(rule.rule_impl_)))
  {
  }

  typedef Domain domain_type;

  rule_impl_type const& get_parameterized_subject() const
  { return this->proto_base().child0.get(); }
  typedef rule_impl_type parameterized_subject_type;
#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, MORBID_GIOP_MAX_ARGS, "morbid/giop/detail/nonterminal_fcall.hpp"))
#include BOOST_PP_ITERATE()

private:
  grammar(grammar const&);
  grammar& operator=(grammar const&);
};

} }

#endif
