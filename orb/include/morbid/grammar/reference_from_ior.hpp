/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GRAMMAR_REFERENCE_FROM_IOR_HPP
#define MORBID_GRAMMAR_REFERENCE_FROM_IOR_HPP

#include <morbid/in_out_traits.hpp>

#include <morbid/ior/grammar/ior.hpp>
#include <morbid/iiop/grammar/profile_body_1_1.hpp>

namespace morbid { namespace grammar {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename T>
struct ref_type {};

BOOST_SPIRIT_TERMINAL_EX(ref_ior)

template <typename T>
struct ref_ior_parser : qi::primitive_parser<ref_ior_parser<T> >
{
  template <typename Context, typename Unused>
  struct attribute
  {
    typedef typename T::_ptr_type type;
  };

  ref_ior_parser(bool little_endian)
    : little_endian(little_endian)
  {}

  template <typename Iterator, typename Context, typename Skipper, typename Attribute>
  bool parse(Iterator& first, Iterator const& last
             , Context& context, Skipper const& skipper
             , Attribute& attr) const
  {
    ior::grammar::ior<Iterator> ior_grammar;
    ior::ior ior_;
    using boost::phoenix::val;
    if(boost::spirit::qi::parse(first, last, ior_grammar(val(first)), ior_))
    {
      // std::cout << "Parsed IOR successfully" << std::endl;
      iiop::grammar::profile_body_1_1<std::vector<char>::const_iterator> profile_body_grammar;
      bool found = false;
      morbid::iiop::profile_body profile_body;
      for(std::vector<ior::tagged_profile>::const_iterator
            first = ior_.profiles.begin()
            , last = ior_.profiles.end()
            ;first != last; ++first)
      {
        if(first->tag == 0 /*TAG_INTERNET_IOP*/)
        {
          if(boost::spirit::qi::parse(first->profile_data.begin()
                                      , first->profile_data.end()
                                      , profile_body_grammar(val(first->profile_data.begin()))
                                      , profile_body))
          {
            // std::cout << "Parsed IIOP profile body information host: " << profile_body.host
            //           << " port: " << profile_body.port 
            //           << " object_key:  " << boost::make_iterator_range(profile_body.object_key.begin()
            //                                                             , profile_body.object_key.end()) << std::endl;
            found = true;
            break;
          }
          else
          {
            std::cout << "Couldn't parse IIOP tagged profile" << std::endl;
          }
        }
      }
      if(found)
      {
        structured_ior ior = {ior_.type_id};
        ior.structured_profiles.push_back(profile_body);
        if(ior_.type_id != T::_repository_id)
        {
          // std::cout << "The type id is not exact, lets ask with is_a" << std::endl;
          Object_ptr p(new ::morbid::remote_stub::Object(ior));
          attr = T::_narrow(p);
        }
        else
        {
          // std::cout << "The type id is exact, we don't need to ask" << std::endl;
          attr = T::_construct_remote_stub(ior);
        }
        return true;
      }
    }
      return false;
  }

  bool little_endian;
};

} }

namespace boost { namespace spirit {

  ///////////////////////////////////////////////////////////////////////////
  // Enablers
  ///////////////////////////////////////////////////////////////////////////

template <typename T>
struct use_terminal
  <qi::domain
   , terminal_ex<morbid::grammar::tag::ref_ior, fusion::vector2< morbid::grammar::ref_type<T>, bool> > >
  : mpl::true_ {};

} } // boost::spirit

namespace boost { namespace spirit { namespace qi {

template <typename Modifiers, typename T>
struct make_primitive
<terminal_ex<morbid::grammar::tag::ref_ior, fusion::vector2< morbid::grammar::ref_type<T>, bool> >
   , Modifiers>
{
  typedef morbid::grammar::ref_ior_parser<T> result_type;
  template <typename Terminal>
  result_type operator()(Terminal const& term, unused_type) const
  {
    return result_type(fusion::at_c<1>(term.args));
  }
};

} } } // boost::spirit::qi

#endif
