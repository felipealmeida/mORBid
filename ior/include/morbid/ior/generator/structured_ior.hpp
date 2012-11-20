/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IOR_GENERATOR_STRUCTURED_IOR_HPP
#define MORBID_IOR_GENERATOR_STRUCTURED_IOR_HPP

#include <morbid/ior/structured_ior.hpp>
#include <morbid/iiop/generator/string.hpp>
#include <morbid/iiop/generator/sequence.hpp>
#include <morbid/iiop/generator/align.hpp>

#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace ior { namespace generator {

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

template <typename OutputIterator, typename StructuredIOR>
struct structured_ior_generator : karma::grammar<OutputIterator, StructuredIOR(bool)>
{
  structured_ior_generator() : structured_ior_generator::base_type(start)
  {
    using karma::_r1;
    using karma::_1;
    using karma::_val; using karma::_a; using karma::_b;
    using phoenix::at_c; using phoenix::if_else;

    start =
      iiop::generator::alignable
      [
       (
        karma::eps(_r1) << karma::lit('\1')
        | karma::lit('\0')
       )
       << iiop::generator::align(4u)
       // type id
       << string(_r1)[_1 = at_c<0>(_val)]
       << iiop::generator::align(4u)
       // number of profiles
       << dword(_r1)[_1 = phoenix::size(phoenix::at_c<1>(_val))]
       // structured profiles
       << (
           +
           (
            iiop::generator::align(4u)
            << profile(_r1)
           )
           )[_1 = at_c<1>(_val)]
      ]
      ;
    profile %=
      iiop_profile_body(_r1)
        | tagged_profile(_r1)
      ;
    tagged_profile = karma::eps;
    iiop_profile_body =
      iiop::generator::alignable
      [
       // profile tag
       dword(_r1)[_1 = 0u]
       // size of everything here down
       << dword(_r1)
       [
        (_a = phoenix::size(phoenix::at_c<0>(_val))+4)
        , (_b = phoenix::size(phoenix::at_c<2>(_val))+4)
        , (_1 = if_else(_a % 4u, (4 - (_a % 4u)) + _a, _a)
           + if_else(_b % 4u, (4 - (_b % 4u)) + _b, _b)
           + 8)
       ]
       << karma::lit('\1')
       << karma::lit('\0')
       << iiop::generator::align(4u)
       // host
       << string(_r1)[_1 = at_c<0>(_val)]
       // port
       << iiop::generator::align(4u)
       << word(_r1)[_1 = at_c<1>(_val)]
       << iiop::generator::align(4u)
       // object key
       << /*karma::as<std::string>()[*/octet_sequence(_r1)/*]*/[_1 = at_c<2>(_val)]
       << iiop::generator::align(4u)
      ]
      ;
  }

  typedef typename StructuredIOR::variant_type variant_type;

  iiop::generator::sequence<OutputIterator, char> octet_sequence;
  iiop::generator::string<OutputIterator> string;
  iiop::generator::dword<OutputIterator> dword;
  iiop::generator::word<OutputIterator> word;
  karma::rule<OutputIterator, StructuredIOR(bool)> start;
  karma::rule<OutputIterator, variant_type(bool)> profile;
  karma::rule<OutputIterator, iiop::profile_body(bool)
              , karma::locals<unsigned int, unsigned int> > iiop_profile_body;
  karma::rule<OutputIterator, morbid::ior::tagged_profile(bool)> tagged_profile;
};

} } }

#endif
