/* (c) Copyright 2012,2013 Felipe Magno de Almeida
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

namespace parser {

template <std::size_t N>
struct unroll_tag {};

template <typename Iterator, typename Value, std::size_t E>
bool unroll_copy(Iterator& first, Iterator last, Value& v, unroll_tag<E>, unroll_tag<E>)
{
  // std::cout << "All copied, value " << v << std::endl;
  return true; 
}

template <typename Iterator, typename Value, std::size_t N, std::size_t E>
bool unroll_copy(Iterator& first, Iterator last, Value& v, unroll_tag<N>, unroll_tag<E> e)
{
  if(first != last)
  {
    static_cast<unsigned char*>(static_cast<void*>(&v))[N] = *first;
    ++first;
    return unroll_copy(first, last, v, unroll_tag<N+1>(), e);
  }
  else
    return false;
}

template <typename Iterator, typename Value>
bool unroll_copy_backward(Iterator& first, Iterator last, Value& v, unroll_tag<0>) { return true; }

template <typename Iterator, typename Value, std::size_t N>
bool unroll_copy_backward(Iterator& first, Iterator last, Value& v, unroll_tag<N>)
{
  if(first != last)
  {
    static_cast<unsigned char*>(static_cast<void*>(&v))[N-1] = *first;
    ++first;
    return unroll_copy_backward(first, last, v, unroll_tag<N-1>());
  }
  else
    return false;
}

template <std::size_t N, typename Iterator, typename T>
bool reverse_unsigned_parse(Iterator& first, Iterator last, T& value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(T), ==, N/CHAR_BIT);
  return unroll_copy_backward(first, last, value, unroll_tag<N/CHAR_BIT>());
}

template <std::size_t N, typename Iterator, typename T>
bool normal_unsigned_parse(Iterator& first, Iterator last, T& value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(T), ==, N/CHAR_BIT);
  return unroll_copy(first, last, value, unroll_tag<0u>(), unroll_tag<N/CHAR_BIT>());
}

template <std::size_t N>
struct unsigned_parser : qi::primitive_parser<unsigned_parser<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<N>::least type;
  };
  
  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    // Should align
    if(!alignment_padding<N>(first, last, ctx.attributes))
      return false;

    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
    
#ifdef BOOST_BIG_ENDIAN
    if(endianness)
#elif defined(BOOST_LITTLE_ENDIAN)
    if(!endianness)
#else
#error No native endianness found
#endif
      return reverse_unsigned_parse<N>(first, last, attr);
    else
      return normal_unsigned_parse<N>(first, last, attr);
  }
};

template <std::size_t N>
struct specific_unsigned_parser : qi::primitive_parser<specific_unsigned_parser<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef spirit::unused_type type;
  };

  specific_unsigned_parser(typename boost::uint_t<N>::least value)
    : value(value) {}
  
  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    // std::cout << "specific_unsigned_parser::parse" << std::endl;
    // Should align
    if(!alignment_padding<N>(first, last, ctx.attributes))
      return false;

    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);

    typename boost::uint_t<N>::least v;
    bool r;
#ifdef BOOST_BIG_ENDIAN
    if(endianness)
#elif defined(BOOST_LITTLE_ENDIAN)
    if(!endianness)
#else
#error No native endianness found
#endif
    {
      r = reverse_unsigned_parse<N>(first, last, v);
    }
    else
    {
      r = normal_unsigned_parse<N>(first, last, v);
    }
    // std::cout << "Value read " << v << " expecting value " << value << std::endl;
    return r && v == value;
  }

  typename boost::uint_t<N>::least value;
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ushort_, Modifiers, Enable>
{
  typedef unsigned_parser<16u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ulong_, Modifiers, Enable>
{
  typedef unsigned_parser<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename U, typename Modifiers, typename Enable>
struct make_primitive<spirit::terminal_ex<spirit::tag::ulong_, fusion::vector1<U> >
                      , Modifiers, Enable>
{
  typedef specific_unsigned_parser<32u> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::ulong_long, Modifiers, Enable>
{
  typedef unsigned_parser<64u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::float_, Modifiers, Enable>
{
  typedef unsigned_parser<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::double_, Modifiers, Enable>
{
  typedef unsigned_parser<64u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::short_, Modifiers, Enable>
{
  typedef unsigned_parser<16u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::long_, Modifiers, Enable>
{
  typedef unsigned_parser<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::long_long, Modifiers, Enable>
{
  typedef unsigned_parser<64u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

namespace generator {


template <std::size_t N>
struct unroll_tag {};

template <typename OutputIterator, typename Value, std::size_t E>
void unroll_copy(OutputIterator& sink, Value const& v, unroll_tag<E>, unroll_tag<E>) {}

template <typename OutputIterator, typename Value, std::size_t N, std::size_t E>
void unroll_copy(OutputIterator& sink, Value const& v, unroll_tag<N>, unroll_tag<E> e)
{
  *sink = static_cast<unsigned char const*>(static_cast<void const*>(&v))[N];
  // std::cout << "copied byte (" << N << ")"
  //           << (unsigned int)static_cast<unsigned char const*>(static_cast<void const*>(&v))[N]
  //           << std::endl;
  ++sink;
  unroll_copy(sink, v, unroll_tag<N+1>(), e);
}

template <typename OutputIterator, typename Value>
void unroll_copy_backward(OutputIterator& sink, Value const& v, unroll_tag<0>)
{
  *sink = static_cast<unsigned char const*>(static_cast<void const*>(&v))[0];
  // std::cout << "copied byte (0) (switched endianness) "
  //           << (unsigned int)static_cast<unsigned char const*>(static_cast<void const*>(&v))[0]
  //           << std::endl;
  ++sink;
}

template <typename OutputIterator, typename Value, std::size_t N>
void unroll_copy_backward(OutputIterator& sink, Value const& v, unroll_tag<N>)
{
  *sink = static_cast<unsigned char const*>(static_cast<void const*>(&v))[N];
  // std::cout << "copied byte (" << N <<  ") (switched endianness) "
  //           << (unsigned int)static_cast<unsigned char const*>(static_cast<void const*>(&v))[N]
  //           << std::endl;
  ++sink;
  unroll_copy_backward(sink, v, unroll_tag<N-1>());
}

template <std::size_t N, typename OutputIterator, typename T>
void reverse_unsigned_generate(OutputIterator& sink, T value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(T), ==, N/CHAR_BIT);
  unroll_copy_backward(sink, value, unroll_tag<N/CHAR_BIT - 1>());
}

template <std::size_t N, typename OutputIterator, typename T>
void normal_unsigned_generate(OutputIterator& sink, T value)
{
  BOOST_MPL_ASSERT_RELATION(sizeof(T), ==, N/CHAR_BIT);
  unroll_copy(sink, value, unroll_tag<0u>(), unroll_tag<N/CHAR_BIT>());
}

template <std::size_t N>
struct unsigned_generator : karma::primitive_generator<unsigned_generator<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<N>::least type;
  };

  typedef mpl::size_t<N/CHAR_BIT> static_size;
  typedef static_size alignment;

  template <typename OutputIterator, typename Context, typename Delimiter>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, spirit::unused_type attr) const
  {
    // It is not possible (doesn't make sense) to use unsigned without
    // providing any attribute, as the generator doesn't 'know' what
    // number to output. The following assertion fires if this
    // situation is detected in your code.
    //BOOST_SPIRIT_ASSERT_MSG(false, unsigned_not_usable_without_attribute, ());
    
    return false;
  }

  template <typename OutputIterator, typename Context, typename Delimiter, typename U>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, U& attr) const
  {
    BOOST_MPL_ASSERT_RELATION(sizeof(U), ==, N/CHAR_BIT);

    // std::cout << "generating unsigned of value " << attr << std::endl;

    alignment_padding<N>(sink, ctx.attributes);
    typedef typename attribute<Context, OutputIterator>::type attribute_type;

    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
#ifdef BOOST_BIG_ENDIAN
    if(endianness)
#elif defined(BOOST_LITTLE_ENDIAN)
    if(!endianness)
#else
#error No native endianness found
#endif
      reverse_unsigned_generate<N>(sink, attr);
    else
      normal_unsigned_generate<N>(sink, attr);
    return true;
  }
};

template <std::size_t N>
struct specific_unsigned_generator : karma::primitive_generator<specific_unsigned_generator<N> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef spirit::unused_type type;
  };

  specific_unsigned_generator(typename boost::uint_t<N>::least value)
    : value(value) {}
  
  template <typename OutputIterator, typename Context, typename Delimiter, typename Attr>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, Attr const& attr) const
  {
    // std::cout << "generating unsigned of value (specificied) " << value << std::endl;

    alignment_padding<N>(sink, ctx.attributes);
    typedef typename attribute<Context, OutputIterator>::type attribute_type;

    bool endianness = generator_endianness<typename Context::attributes_type>
      ::call(ctx.attributes);
#ifdef BOOST_BIG_ENDIAN
    if(endianness)
#elif defined(BOOST_LITTLE_ENDIAN)
    if(!endianness)
#else
#error No native endianness found
#endif
      reverse_unsigned_generate<N>(sink, value);
    else
      normal_unsigned_generate<N>(sink, value);
    return true;
  }

  typename boost::uint_t<N>::least value;
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
struct make_primitive<spirit::tag::float_, Modifiers, Enable>
{
  typedef unsigned_generator<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::double_, Modifiers, Enable>
{
  typedef unsigned_generator<64u> result_type;

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
struct make_primitive<spirit::tag::long_, Modifiers, Enable>
{
  typedef unsigned_generator<32u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<spirit::tag::short_, Modifiers, Enable>
{
  typedef unsigned_generator<16u> result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename U, typename Modifiers, typename Enable>
struct make_primitive<spirit::terminal_ex<spirit::tag::ulong_, fusion::vector1<U> >
                      , Modifiers, Enable>
{
  typedef specific_unsigned_generator<32u> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
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
template <typename U, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , terminal_ex<tag::ulong_, fusion::vector1<U> >, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::ulong_long, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::short_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::long_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::long_long, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::double_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, tag::float_, Enable> : mpl::true_ {};

// Parser
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::ushort_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::ulong_, Enable> : mpl::true_ {};
template <typename U, typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain
                     , terminal_ex<tag::ulong_, fusion::vector1<U> >, Enable> : mpl::true_ {};
template <>
struct use_lazy_terminal< ::morbid::iiop::parser_domain, tag::ulong_, 1> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::ulong_long, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::short_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::long_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::long_long, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::double_, Enable> : mpl::true_ {};
template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, tag::float_, Enable> : mpl::true_ {};

} }

#endif
