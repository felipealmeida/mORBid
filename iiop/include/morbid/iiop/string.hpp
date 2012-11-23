/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_STRING_HPP
#define MORBID_IIOP_STRING_HPP

#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/numeric.hpp>
#include <morbid/iiop/octet.hpp>

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
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d, C& attr) const
  {
    unsigned_generator<32u> unsigned_;
    boost::uint_t<32>::least size = attr.size()+1;
    bool r = unsigned_.generate(sink, ctx, d, size);
    karma::any_char<octet_encoding, spirit::unused_type> octet_generator;
    for(typename spirit::traits::container_iterator<C>::type 
          first = spirit::traits::begin(attr)
          , last = spirit::traits::end(attr)
          ;first != last; ++first)
    {
      octet_encoding::char_type c = *first;
      octet_generator.generate(sink, ctx, d, c);
    }
    octet_encoding::char_type zero = '\0';
    octet_generator.generate(sink, ctx, d, zero);
    return r;
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
