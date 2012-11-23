/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_NUMERIC_HPP
#define MORBID_IIOP_NUMERIC_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/endianness.hpp>
#include <morbid/iiop/alignment.hpp>

#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

template <std::size_t N>
struct unroll_tag {};

template <typename OutputIterator, typename Value, std::size_t E>
void unroll_copy(OutputIterator& sink, Value const& v, unroll_tag<E>, unroll_tag<E>) {}

template <typename OutputIterator, typename Value, std::size_t N, std::size_t E>
void unroll_copy(OutputIterator& sink, Value const& v, unroll_tag<N>, unroll_tag<E> e)
{
  sink = static_cast<unsigned char const*>(static_cast<void const*>(&v))[N];
  unroll_copy(sink, v, unroll_tag<N+1>(), e);
}

template <typename OutputIterator, typename Value>
void unroll_copy_backward(OutputIterator& sink, Value const& v, unroll_tag<0>) {}

template <typename OutputIterator, typename Value, std::size_t N>
void unroll_copy_backward(OutputIterator& sink, Value const& v, unroll_tag<N>)
{
  sink = static_cast<unsigned char const*>(static_cast<void const*>(&v))[N];
  unroll_copy_backward(sink, v, unroll_tag<N-1>());
}

template <std::size_t N, typename OutputIterator>
bool little_endian_unsigned_generate(OutputIterator& sink, typename boost::uint_t<N>::exact value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(typename boost::uint_t<N>::exact), ==, N/CHAR_BIT);
  unroll_copy_backward(sink, value, unroll_tag<N/CHAR_BIT>());
  return true;
}

template <std::size_t N, typename OutputIterator>
bool big_endian_unsigned_generate(OutputIterator& sink, typename boost::uint_t<N>::exact value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(typename boost::uint_t<N>::exact), ==, N/CHAR_BIT);
  unroll_copy(sink, value, unroll_tag<0u>(), unroll_tag<N/CHAR_BIT>());
  return true;
}

template <std::size_t N>
struct unsigned_generator : karma::primitive_generator<unsigned_generator<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<N>::least type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename U>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, U& attr) const
  {
    alignment_padding<N>(sink, ctx.attributes);

    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
    return endianness ? little_endian_unsigned_generate<N>(sink, attr)
      : big_endian_unsigned_generate<N>(sink, attr);
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
