/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_ARGUMENTS_TRAITS_HPP
#define MORBID_ORB_ARGUMENTS_TRAITS_HPP

#include <morbid/struct_fusion.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/giop/common_terminals.hpp>
#include <morbid/reference_giop.hpp>
#include <morbid/reference.hpp>

#include <boost/proto/core.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid {

namespace spirit = boost::spirit;

namespace detail {

template <typename T, typename Domain, typename Iterator, typename Enable = void>
struct argument_giop_grammar;

template <typename Domain, typename Iterator>
struct argument_giop_grammar<morbid::string, Domain, Iterator>
{
  typedef spirit::terminal<giop::tag::string>const& result_type;
  result_type operator()(orb) const { return giop::string; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<char, Domain, Iterator>
{
  typedef spirit::terminal<giop::tag::octet>const& result_type;
  result_type operator()(orb) const { return giop::octet; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<bool, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::bool_>const& result_type;
  result_type operator()(orb) const { return giop::bool_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<double, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::double_>const& result_type;
  result_type operator()(orb) const { return giop::double_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<float, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::float_>const& result_type;
  result_type operator()(orb) const { return giop::float_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<long_long, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::long_long>const& result_type;
  result_type operator()(orb) const { return giop::long_long; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<unsigned_long_long, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::ulong_long>const& result_type;
  result_type operator()(orb) const { return giop::ulong_long; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<long_, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::long_>const& result_type;
  result_type operator()(orb) const { return giop::long_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<unsigned_long, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::ulong_>const& result_type;
  result_type operator()(orb) const { return giop::ulong_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<short_, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::short_>const& result_type;
  result_type operator()(orb) const { return giop::short_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<unsigned_short, Domain, Iterator>
{
  typedef spirit::terminal<spirit::tag::ushort_>const& result_type;
  result_type operator()(orb) const { return giop::ushort_; }
};

template <typename Domain, typename Iterator>
struct argument_giop_grammar<octet, Domain, Iterator>
{
  typedef spirit::terminal<giop::tag::octet>const& result_type;
  result_type operator()(orb) const { return giop::octet; }
};

template <typename T, typename Domain, typename Iterator>
struct argument_giop_grammar<std::vector<T>, Domain, Iterator>
{
  // typedef spirit::terminal<giop::tag::octet>const& result_type;

  // typedef boost::proto::exprns_::expr

  typedef boost::proto::exprns_::expr
  <
    boost::proto::tagns_::tag::subscript
    , boost::proto::argsns_::list2
    <boost::spirit::terminal<morbid::giop::tag::sequence>const&
     , typename argument_giop_grammar<T, Domain, Iterator>::result_type
    >
    , 2l
  > const result_type;

  // const boost::proto::exprns_::expr
  // <
  //   boost::proto::tagns_::tag::subscript
  //   , boost::proto::argsns_::list2
  //   <const boost::spirit::terminal<morbid::giop::tag::sequence>&
  //    , const boost::spirit::terminal<boost::spirit::tag::bool_>&
  //   >
  // , 2l>

  result_type operator()(struct orb orb) const
  {
    return giop::sequence[argument_giop_grammar<T, Domain, Iterator>()(orb)];
  }
};

template <typename T, typename Domain, typename Iterator>
struct argument_giop_grammar<T, Domain, Iterator
                             , typename boost::enable_if<boost::is_same<typename T::_morbid_type_kind, struct_tag> >::type>
{
  typedef typename T::template _morbid_grammar<Domain, Iterator, fusion_adapt::struct_sequence<T> > grammar_type;
  typedef grammar_type const& result_type;
  result_type operator()(orb) const
  {
    static grammar_type r;
    return r;
  }
};

template <typename T, typename Domain, typename Iterator>
struct argument_giop_grammar< reference<T>, Domain, Iterator>
{
  typedef boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal
                                      , boost::proto::argsns_::term
                                      <boost::spirit::terminal_ex
                                       <morbid::giop::tag::reference
                                        , boost::fusion::vector1<morbid::orb> > >, 0l>const result_type;
  result_type operator()(morbid::orb orb) const
  {
    return giop::reference(orb);
  }
};

// template <typename T/*, typename Domain*/, typename Iterator>
// struct argument_giop_grammar< reference<T>, iiop::parser_domain, Iterator>
// {
//   typedef spirit::terminal<morbid::giop::tag::reference>const& result_type;
//   result_type operator()(morbid::orb orb) const
//   {
//     return giop::reference//(/*boost::phoenix::val(*/orb/*)*/)
//       ;
//   }
// };

// template <typename T, typename Domain, typename Iterator>
// struct argument_giop_grammar< reference<T>, Domain, Iterator>
// {
//   typedef spirit::terminal<morbid::giop::tag::reference>const& result_type;
//   result_type operator()(morbid::orb orb) const
//   {
//     return giop::reference//(/*boost::phoenix::val(*/orb/*)*/)
//       ;
//   }
// };

}

struct arguments_traits
{
  arguments_traits(orb o)
    : orb_(o) {}

  template <typename T, typename Domain, typename Iterator>
  typename detail::argument_giop_grammar<typename T::original_type, Domain, Iterator>::result_type
  call() const
  {
    return detail::argument_giop_grammar<typename T::original_type, Domain, Iterator>()(orb_);
  }

  orb orb_;
};

}

#endif

