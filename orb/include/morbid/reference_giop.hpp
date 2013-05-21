/* (c) Copyright 2012,2013 Felipe Magno de Almeida
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
#include <morbid/orb.hpp>

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
  reference_generator(struct orb orb)
    : orb_(orb) {}

  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename boost::uint_t<32>::least type;
  };

  template <typename OutputIterator, typename Context, typename Delimiter, typename U>
  bool generate(OutputIterator& sink, Context& ctx, Delimiter const&, U& attr) const
  {
    // std::cout << "reference_generator::generate " << typeid(U).name() << std::endl;

    structured_ior sior;
    if(!attr._sior)
    {
      // std::cout << "serve_copy" << std::endl;
      orb::object_id id = orb_.serve_copy
        <typename U::concept_class, typename U::any_type>(attr);
      sior = structured_ior_object_id(orb_, id);
    }
    else
      sior = *attr._sior;
    
    // std::cout << "is a remote reference size: " << sior.structured_profiles.size() << std::endl;

    typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
    typedef structured_ior attribute_type;
    typedef ior::grammar::ior<iiop::generator_domain, output_iterator_type
                              , attribute_type>
      ior_grammar_type;

    ior::grammar::tagged_profile<iiop::generator_domain, output_iterator_type
                                 , ior::tagged_profile> tagged_profile;
    iiop::grammar::profile_body_1_0<iiop::generator_domain, output_iterator_type
                                    , iiop::profile_body> profile_body;
    ior::grammar::generic_tagged_profile<iiop::generator_domain, output_iterator_type
                                         , iiop::profile_body, 0u
                                           > tagged_profile_body
      (giop::endianness[profile_body]);
    
    ior_grammar_type ior_grammar(tagged_profile | tagged_profile_body);
      
    namespace karma = boost::spirit::karma;
    if(karma::generate(sink
                       , giop::compile<iiop::generator_domain>
                       (
                        ior_grammar(giop::native_endian)
                       )
                       , sior))
    {
      // std::cout << "Success" << std::endl;
      return true;
    }
    else
    {
      std::cout << "Failed generating" << std::endl;
      return false;
    }
  }

  struct orb orb_;
};

struct reference_parser : qi::primitive_parser<reference_parser>
{
  reference_parser(orb o)
    : orb_(o) {}

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
    // std::cout << "reference_parser::parse " << typeid(Attribute).name() << std::endl;

    typedef structured_ior attribute_type;
    namespace fusion = boost::fusion;
    typedef typename fusion::result_of::as_vector
      <fusion::joint_view<fusion::single_view<char> // minor version
                          , iiop::profile_body> >::type profile_body_1_1_attr;

    ior::grammar::tagged_profile<iiop::parser_domain, Iterator
                                 , ior::tagged_profile> tagged_profile;
    iiop::grammar::profile_body_1_0<iiop::parser_domain, Iterator
                                    , iiop::profile_body> profile_body_1_0;
    iiop::grammar::profile_body_1_1<iiop::parser_domain, Iterator
                                    , profile_body_1_1_attr> profile_body_1_1;
    ior::grammar::generic_tagged_profile<iiop::parser_domain, Iterator
                                         , boost::variant<iiop::profile_body, profile_body_1_1_attr>, 0u
                                         > tagged_profile_body
      (giop::endianness[profile_body_1_0 | profile_body_1_1]);

    typedef fusion::vector2<std::string
                            , std::vector
                            <boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >
                            > result_type;
    result_type r;

    typedef ior::grammar::ior<iiop::parser_domain, Iterator, result_type>
      ior_grammar_type;

    ior_grammar_type ior_grammar(tagged_profile_body | tagged_profile);
    namespace qi = boost::spirit::qi;
    Iterator first_ = first;
    if(qi::parse(first_, last
                 , giop::compile<iiop::parser_domain>(ior_grammar(giop::native_endian))
                 , r))
    {
      // std::cout << "Parsed OK" << std::endl;

      structured_ior sior = {fusion::at_c<0u>(r)};
    
      for(std::vector<boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >::const_iterator
            first = fusion::at_c<1u>(r).begin()
            , last = fusion::at_c<1u>(r).end(); first != last; ++first)
      {
        if(iiop::profile_body const* p = boost::get<iiop::profile_body>(&*first))
          sior.structured_profiles.push_back(*p);
        else if(ior::tagged_profile const* p = boost::get<ior::tagged_profile>(&*first))
          sior.structured_profiles.push_back(*p);
        else if(profile_body_1_1_attr const* p = boost::get<profile_body_1_1_attr>(&*first))
        {
          iiop::profile_body b;
          fusion::copy(fusion::pop_front(*p), b);
          sior.structured_profiles.push_back(b);
        }
      }

      typedef typename Attribute::concept_class::remote_reference ref;

      attr = ref(orb_, sior);
      first = first_;
      return true;
    }
    else
      return false;
  }

  orb orb_;
};

namespace iiop { namespace generator {

template <typename Modifiers>
struct make_primitive<spirit::terminal_ex<giop::tag::reference, boost::fusion::vector1<morbid::orb> >, Modifiers>
{
  typedef morbid::reference_generator result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Modifiers>
struct make_primitive<giop::tag::reference, Modifiers>
{
  typedef morbid::reference_generator result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

}

namespace parser {

template <typename Modifiers>
struct make_primitive<spirit::terminal_ex<giop::tag::reference, boost::fusion::vector1<morbid::orb> >, Modifiers>
{
  typedef morbid::reference_parser result_type;

  template <typename Terminal>
  result_type operator()(Terminal& term, boost::spirit::unused_type) const
  {
    return result_type(fusion::at_c<0u>(term.args));
  }
};

} } }

namespace boost { namespace spirit {

template <>
struct use_lazy_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::reference, 1>
  : mpl::true_
{};

template <>
struct use_terminal< ::morbid::iiop::generator_domain, morbid::giop::tag::reference>
  : mpl::true_
{};

template <>
struct use_lazy_terminal< ::morbid::iiop::parser_domain, morbid::giop::tag::reference, 1>
  : mpl::true_
{};

} }

#endif
