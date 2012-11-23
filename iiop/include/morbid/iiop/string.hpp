/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_STRING_HPP
#define MORBID_IIOP_STRING_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/giop/common_terminals.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace karma = spirit::karma;

namespace generator {

struct string_generator : karma::primitive_generator<string_generator>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef std::string type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename C>
  bool generate(OutputIterator& sink, Context&, Delimiter const&, C& attr) const
  {
    return false;
  }
};

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::string, Modifiers, Enable>
{
  typedef string_generator result_type;

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
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::string, Enable> : mpl::true_ {};

} }

#endif
