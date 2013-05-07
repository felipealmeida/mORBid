/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_REFERENCE_GIOP_HPP
#define MORBID_REFERENCE_GIOP_HPP

#include <morbid/giop/rule.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
#include <morbid/iiop/domain.hpp>
#include <morbid/iiop/meta_compiler.hpp>
#include <morbid/iiop/grammars/profile_body_1_1.hpp>
#include <morbid/iiop/rule.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/ior/grammar/generic_tagged_profile.hpp>
#include <morbid/ior/grammar/tagged_profile.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/structured_ior.hpp>

#include <boost/spirit/home/support.hpp>
#include <boost/spirit/home/karma.hpp>

namespace morbid { namespace giop {

BOOST_SPIRIT_TERMINAL_EX(reference);

} }

namespace morbid {

namespace karma = boost::spirit::karma;
namespace qi = boost::spirit::qi;

struct reference_generator : karma::primitive_generator<reference_generator>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<32>::least type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename U>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, U& attr) const
  {
    std::cout << "reference_generator::generate" << typeid(U).name() << std::endl;

    if(attr.remote)
      std::cout << "is a remote reference " << typeid(U).name() << std::endl;
    else
      std::cout << "is NOT a remote reference " << typeid(U).name() << std::endl;
    // structured_ior sior = attr._structured_ior();

    // typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
    // typedef structured_ior attribute_type;
    // typedef ior::grammar::ior<iiop::generator_domain, output_iterator_type
    //                           , attribute_type>
    //   ior_grammar_type;

    // ior::grammar::tagged_profile<iiop::generator_domain, output_iterator_type
    //                              , ior::tagged_profile> tagged_profile;
    // iiop::grammar::profile_body_1_0<iiop::generator_domain, output_iterator_type
    //                                 , iiop::profile_body> profile_body;
    // ior::grammar::generic_tagged_profile<iiop::generator_domain, output_iterator_type
    //                                      , iiop::profile_body, 0u
    //                                      > tagged_profile_body
    //   (giop::endianness[profile_body]);
    
    // ior_grammar_type ior_grammar(tagged_profile | tagged_profile_body);

    // namespace karma = boost::spirit::karma;
    // if(karma::generate(sink
    //                    , giop::compile<iiop::generator_domain>
    //                    (
    //                     ior_grammar(giop::native_endian)
    //                    )
    //                    , sior))
    // {
    //   std::cout << "Success" << std::endl;
    //   return true;
    // }
    // else
      return false;
  }  
};

struct reference_parser : qi::primitive_parser<reference_parser>
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<32>::least type;
  };

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& ctx, Skipper const& skipper
             , Attribute& attr) const
  {
    std::cout << "reference_parser::parse " << typeid(Attribute).name() << std::endl;

    // typedef structured_ior attribute_type;
    // namespace fusion = boost::fusion;
    // typedef typename fusion::result_of::as_vector
    //   <fusion::joint_view<fusion::single_view<char> // minor version
    //                       , iiop::profile_body> >::type profile_body_1_1_attr;

    // ior::grammar::tagged_profile<iiop::parser_domain, Iterator
    //                              , ior::tagged_profile> tagged_profile;
    // iiop::grammar::profile_body_1_0<iiop::parser_domain, Iterator
    //                                 , iiop::profile_body> profile_body_1_0;
    // iiop::grammar::profile_body_1_1<iiop::parser_domain, Iterator
    //                                 , profile_body_1_1_attr> profile_body_1_1;
    // ior::grammar::generic_tagged_profile<iiop::parser_domain, Iterator
    //                                      , boost::variant<iiop::profile_body, profile_body_1_1_attr>, 0u
    //                                      > tagged_profile_body
    //   (giop::endianness[profile_body_1_0 | profile_body_1_1]);

    // typedef fusion::vector2<std::string
    //                         , std::vector
    //                         <boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >
    //                         > result_type;
    // result_type r;

    // typedef ior::grammar::ior<iiop::parser_domain, Iterator, result_type>
    //   ior_grammar_type;

    // ior_grammar_type ior_grammar(tagged_profile_body | tagged_profile);
    // namespace qi = boost::spirit::qi;
    // Iterator first_ = first;
    // if(qi::parse(first_, last
    //              , giop::compile<iiop::parser_domain>(ior_grammar(giop::native_endian))
    //              , r))
    // {
    //   first = first_;
    //   std::cout << "Parsed OK" << std::endl;

    //   structured_ior sior = {fusion::at_c<0u>(r)};
    
    //   for(std::vector<boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >::const_iterator
    //         first = fusion::at_c<1u>(r).begin()
    //         , last = fusion::at_c<1u>(r).end(); first != last; ++first)
    //   {
    //     if(iiop::profile_body const* p = boost::get<iiop::profile_body>(&*first))
    //       sior.structured_profiles.push_back(*p);
    //     else if(ior::tagged_profile const* p = boost::get<ior::tagged_profile>(&*first))
    //       sior.structured_profiles.push_back(*p);
    //     else if(profile_body_1_1_attr const* p = boost::get<profile_body_1_1_attr>(&*first))
    //     {
    //       iiop::profile_body b;
    //       fusion::copy(fusion::pop_front(*p), b);
    //       sior.structured_profiles.push_back(b);
    //     }
    //   }

    //   typedef typename Attribute::element_type element_type;

    //   attr = element_type::_construct_remote_stub(sior);
    //   return true;
    // }
    // else
      return false;
  }  
};

namespace iiop { namespace generator {

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::reference, Modifiers, Enable>
{
  typedef morbid::reference_generator result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

}

namespace parser {

template <typename Modifiers, typename Enable>
struct make_primitive<giop::tag::reference, Modifiers, Enable>
{
  typedef morbid::reference_parser result_type;

  template <typename T_>
  result_type operator()(T_& val, boost::spirit::unused_type) const
  {
    return result_type();
  }
};

} } }

namespace boost { namespace spirit {

template <typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::reference, Enable>
  : mpl::true_ {};

template <typename Enable>
struct use_terminal< ::morbid::iiop::parser_domain, morbid::giop::tag::reference, Enable>
  : mpl::true_ {};

} }

#endif
