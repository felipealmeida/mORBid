/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_NUMERIC_HPP
#define MORBID_IIOP_NUMERIC_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

template <std::size_t N>
struct unsigned_generator : karma::primitive_generator<unsigned_generator<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<N>::least type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename U>
  bool generate(OutputIterator& sink, Context&, Delimiter const&, U& attr) const
  {
    return false;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ushort_, Modifiers, Enable>
{
  typedef unsigned_generator<16u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ulong_, Modifiers, Enable>
{
  typedef unsigned_generator<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ulong_long, Modifiers, Enable>
{
  typedef unsigned_generator<64u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

} }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::ushort_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::ulong_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::ulong_long, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::short_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::long_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::long_long, Enable> : mpl::true_ {};

} }

#endif
