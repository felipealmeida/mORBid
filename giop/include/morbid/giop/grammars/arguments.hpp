/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef MORBID_GIOP_GRAMMARS_ARGUMENTS_HPP
#define MORBID_GIOP_GRAMMARS_ARGUMENTS_HPP

#include <morbid/giop/grammar.hpp>
#include <morbid/giop/detail/max_args.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>

namespace morbid { namespace giop { namespace grammars {

namespace fusion = boost::fusion;
namespace proto = boost::proto;
namespace mpl = boost::mpl;
namespace spirit = boost::spirit;
namespace karma = boost::spirit::karma;

template <std::size_t AttrSize, typename Domain, typename Iterator, typename TypeSeq, typename Attr>
struct arguments_aux;

template <typename Domain, typename Iterator, typename TypeSeq, typename Attr>
struct arguments_aux<0u, Domain, Iterator, TypeSeq, Attr> : grammar<Domain, Iterator, Attr(endian)>
{
  template <typename C>
  arguments_aux(C const& c) : arguments_aux::base_type(start)
  {
    start = spirit::eps;
  }

  rule<Domain, Iterator, Attr(endian)> start;
};

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, MORBID_GIOP_MAX_ARGS, "morbid/giop/grammars/arguments.hpp"))
#include BOOST_PP_ITERATE()

template <typename Domain, typename Iterator, typename TypeSeq, typename Attr>
struct arguments : arguments_aux<mpl::size<TypeSeq>::value, Domain, Iterator, TypeSeq, Attr>
{
  typedef arguments_aux<mpl::size<TypeSeq>::value, Domain, Iterator, TypeSeq, Attr> base_type;
  template <typename C>
  arguments(C const& c)
    : base_type(c) {}
};


} } }

#endif
#else

#define N BOOST_PP_ITERATION()

template <typename Domain, typename Iterator, typename TypeSeq, typename Attr>
struct arguments_aux<N, Domain, Iterator, TypeSeq, Attr> : grammar<Domain, Iterator, Attr(endian)>
{
  template <typename C>
  arguments_aux(C const& c) : arguments_aux::base_type(start)
  {
#define MORBID_GIOP_GRAMMARS_ARGUMENTS_sequence(Z, I, DATA)     \
    BOOST_PP_EXPR_IF(BOOST_PP_DEC(N), &) c.template call<typename mpl::at_c<TypeSeq, I>::type>()

    start = BOOST_PP_REPEAT(N, MORBID_GIOP_GRAMMARS_ARGUMENTS_sequence, ~)
      ;

#undef MORBID_GIOP_GRAMMARS_ARGUMENTS_sequence
  }

  rule<Domain, Iterator, Attr(endian)> start;
};

#undef N
#endif
