/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_ENDIANNESS_HPP
#define MORBID_IIOP_ENDIANNESS_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/giop/common_terminals.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

template <typename Subject>
struct endianness_generator : karma::unary_generator<endianness_generator<Subject> >
{
  template <typename Context, typename Iterator>
  struct attribute
  {
    typedef
      typename spirit::traits::attribute_of<
        Subject, Context, Iterator>::type
    type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context&, Delimiter const&, C& attr) const
  {
    return false;
  }

  endianness_generator(Subject const& subject)
    : subject(subject) 
  {
  }

  Subject subject;
};

template <typename Subject, typename Modifiers>
struct make_directive<giop::tag::endianness, Subject, Modifiers>
{
  typedef endianness_generator<Subject> result_type;

  result_type operator()(spirit::unused_type, Subject const& subject, boost::spirit::unused_type) const
  {
    return result_type(subject);
  }
};

}

} }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, Enable> : mpl::true_ {};

template <typename Enable>
struct use_directive< ::morbid::iiop::generator_domain, morbid::giop::tag::endianness, Enable> : mpl::true_ {};

} }

#endif
