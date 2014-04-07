/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_WAVE_STRING
#define MORBID_IDL_COMPILER_WAVE_STRING

#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/delimit_out.hpp>
#include <boost/spirit/home/support/terminal.hpp>
#include <boost/spirit/home/support/char_encoding/ascii.hpp>
#include <boost/spirit/home/karma/generator.hpp>
#include <boost/spirit/home/karma/detail/string_generate.hpp>
#include <boost/spirit/home/karma/detail/get_casetag.hpp>
#include <boost/spirit/home/karma/detail/extract_from.hpp>
#include <boost/spirit/home/support/detail/get_encoding.hpp>
#include <morbid/idl_parser/wave_string.hpp>

namespace morbid { namespace idlc {

BOOST_SPIRIT_TERMINAL(wave_string)

template <typename CharEncoding, typename Tag>
struct wave_string_generator
  : boost::spirit::karma::primitive_generator<wave_string_generator<CharEncoding, Tag> >
{
  typedef idl_parser::wave_string string_type;
  typedef CharEncoding char_encoding;
  typedef char char_type;

  template <typename Context, typename Unused = boost::spirit::unused_type>
  struct attribute
  {
    typedef string_type type;
  };

  // lit has an attached attribute
  template <typename OutputIterator, typename Context, typename Delimiter
            , typename Attribute>
  static bool
  generate(OutputIterator& sink, Context& context, Delimiter const& d, 
           Attribute const& attr)
  {
    if (!boost::spirit::traits::has_optional_value(attr))
      return false;

    namespace spirit = boost::spirit;
    namespace karma = spirit::karma;
    namespace traits = spirit::traits;
    
    typedef typename attribute<Context>::type attribute_type;
    return  karma::detail::string_generate
      (sink, traits::extract_from<attribute_type>(attr, context)
       , char_encoding(), Tag()) && karma::delimit_out(sink, d);      // always do post-delimiting
  }

  // this lit has no attribute attached, it needs to have been
  // initialized from a direct literal
  template <typename OutputIterator, typename Context, typename Delimiter>
  static bool generate(OutputIterator&, Context&, Delimiter const&, 
                       boost::spirit::unused_type const&)
  {
    // It is not possible (doesn't make sense) to use string without
    // providing any attribute, as the generator doesn't 'know' what
    // character to output. The following assertion fires if this
    // situation is detected in your code.
    BOOST_SPIRIT_ASSERT_FAIL(OutputIterator, string_not_usable_without_attribute, ());
    return false;
  }

  template <typename Context>
  static boost::spirit::info what(Context const& /*context*/)
  {
    return boost::spirit::info("wave-string");
  }
};

} }

namespace boost { namespace spirit { 

template <>
struct use_terminal<karma::domain
                    , ::morbid::idlc::tag::wave_string>
  : boost::mpl::true_
{};

namespace karma {

template <typename Modifiers>
struct make_primitive< ::morbid::idlc::tag::wave_string, Modifiers>
{
  static bool const lower = 
    has_modifier<Modifiers, tag::char_code_base<tag::lower> >::value;
  static bool const upper = 
    has_modifier<Modifiers, tag::char_code_base<tag::upper> >::value;
  typedef ::morbid::idlc::wave_string_generator
  <
    typename spirit::detail::get_encoding_with_case<
     Modifiers, spirit::char_encoding::ascii, lower || upper>::type
    , typename detail::get_casetag<Modifiers, lower || upper>::type
  > result_type;

  result_type operator()(unused_type, unused_type) const
  {
    return result_type();
  }
};

}

} }

#endif
