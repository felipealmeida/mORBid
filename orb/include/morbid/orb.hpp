/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_INIT_HPP
#define MORBID_ORB_INIT_HPP

#include <morbid/reply.hpp>
#include <morbid/structured_ior.hpp>
#include <morbid/giop/forward_back_insert_iterator.hpp>
#include <morbid/ior/grammar/corbaloc.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/ior/grammar/tagged_profile.hpp>
#include <morbid/ior/grammar/generic_tagged_profile.hpp>
#include <morbid/ior/hex_directive.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/iiop/grammars/profile_body_1_1.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/fusion/include/fused.hpp>
#include <boost/fusion/include/make_fused.hpp>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>

#include <boost/asio.hpp>

BOOST_TYPE_ERASURE_MEMBER((morbid)(poa)(has_call), call, 6)

namespace morbid {

namespace mpl = boost::mpl;

template <typename R, typename SeqParam, typename T, typename F>
void handle_request_body(struct orb orb, T* self, F f, std::size_t align_offset
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r);

namespace poa {

struct connection;
template <typename C, typename T>
struct object_registration;

}

struct orb;

struct orb_impl : boost::enable_shared_from_this<orb_impl>
{
  orb_impl();
  ~orb_impl()
  {
    std::cout << "Destroying orb" << std::endl;
  }

  void run();
  void stop();
  void handle_accept(boost::shared_ptr<poa::connection> c);

  std::size_t last_id;

  typedef boost::type_erasure::any
    <boost::mpl::vector
     <boost::type_erasure::copy_constructible<>
      , poa::has_call<void(std::string, std::size_t, const char*
                           , const char*, bool
                           , reply&
                           )>
     > > object;
  struct object_registration
  {
    object_registration(std::string const& type_id
                        , object const& obj)
      : type_id(type_id), obj(obj) {}
    std::string type_id;
    object obj;
  };

  typedef std::map<std::size_t, object_registration> object_map_type;
  typedef object_map_type::iterator object_map_iterator;
  object_map_type object_map;
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::ip::tcp::endpoint local_endpoint;
};

struct orb
{
  orb() : impl(new orb_impl) {}

  void run() { impl->run(); }
  void stop() { impl->stop(); }

  typedef orb_impl::object_map_iterator object_id;

  template <typename C, typename T>
  object_id serve_ref(T& servant) const
  {
    std::pair<object_id, bool> pair = impl->object_map.insert
      (std::make_pair(impl->last_id++, orb_impl::object_registration
                      (C::type_id()
                       , poa::object_registration<C, T&>(*this, servant))));
    assert(pair.second);
    std::cout << "Added object " << pair.first->first << std::endl;
    return pair.first;
  }

  template <typename C, typename T>
  object_id serve_copy(T servant) const
  {
    std::pair<object_id, bool> pair = impl->object_map.insert
      (std::make_pair(impl->last_id++, orb_impl::object_registration
                      (C::type_id()
                       , poa::object_registration<C, T>(*this, servant))));
    assert(pair.second);
    std::cout << "Added object " << pair.first->first << std::endl;
    return pair.first;
  }

private:
  template <typename OutputIterator>
  friend void stringify_object_id( morbid::orb orb, orb_impl::object_map_iterator object_id, OutputIterator output);
  friend structured_ior structured_ior_object_id
    (struct orb orb, orb_impl::object_map_iterator object_id);

  boost::shared_ptr<orb_impl> impl;
};

inline structured_ior structured_ior_object_id
  (struct orb orb, orb_impl::object_map_iterator object_id)
{
  iiop::profile_body profile_body("localhost"
                                  , orb.impl->local_endpoint.port()
                                  , std::vector<char>());
  profile_body.object_key.insert(profile_body.object_key.end()
                                 , (unsigned char*)&object_id->first
                                 , (unsigned char*)&object_id->first
                                 + sizeof(object_id->first));
  structured_ior ior = {object_id->second.type_id};
  ior.structured_profiles.push_back(profile_body);
  return ior;
}

template <typename OutputIterator>
void stringify_object_id( morbid::orb orb, orb_impl::object_map_iterator object_id, OutputIterator output)
{
  namespace fusion = boost::fusion;
  namespace karma = boost::spirit::karma;

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef fusion::vector2<std::string, std::vector<iiop::profile_body> > attribute_type;
  typedef ior::grammar::ior<iiop::generator_domain, output_iterator_type, attribute_type>
    ior_grammar_type;

  iiop::grammar::profile_body_1_0<iiop::generator_domain, output_iterator_type
                                  , iiop::profile_body> profile_body;
  ior::grammar::generic_tagged_profile<iiop::generator_domain, output_iterator_type
                                       , iiop::profile_body, 0u
                                       > tagged_profile_body
    (giop::endianness[profile_body]);

  ior_grammar_type ior_grammar(tagged_profile_body);
  
  std::vector<iiop::profile_body> profile_bodies(1);
  profile_bodies[0].host = "localhost";
  profile_bodies[0].port = orb.impl->local_endpoint.port();
  profile_bodies[0].object_key.insert(profile_bodies[0].object_key.end()
                                      , (unsigned char*)&object_id->first
                                      , (unsigned char*)&object_id->first
                                      + sizeof(object_id->first));
  std::cout << "Object key: " << object_id->first << std::endl;

  std::vector<char> ior;
  output_iterator_type iterator(ior);
  if(karma::generate(iterator
                     , karma::lit("IOR:")
                     << ior::hex
                     [
                      giop::compile<iiop::generator_domain>
                      (giop::endianness(giop::native_endian)[ior_grammar])
                     ]
                     , fusion::make_vector(object_id->second.type_id, profile_bodies) ))
  {
    std::copy(ior.begin(), ior.end(), output);
  }
  else
    throw std::runtime_error("BUG: Error stringifying object_id");
}

template <typename Iterator>
structured_ior string_to_structured_ior(Iterator first, Iterator last)
{
  typedef std::vector<char> hex_parser_container;
  typedef hex_parser_container::const_iterator hex_iterator;

  typedef structured_ior attribute_type;
  namespace fusion = boost::fusion;
  typedef fusion::result_of::as_vector
    <fusion::joint_view<fusion::single_view<char> // minor version
                        , iiop::profile_body> >::type profile_body_1_1_attr;

  ior::grammar::tagged_profile<iiop::parser_domain, hex_iterator
                               , ior::tagged_profile> tagged_profile;
  iiop::grammar::profile_body_1_0<iiop::parser_domain, hex_iterator
                                  , iiop::profile_body> profile_body_1_0;
  iiop::grammar::profile_body_1_1<iiop::parser_domain, hex_iterator
                                  , profile_body_1_1_attr> profile_body_1_1;
  ior::grammar::generic_tagged_profile<iiop::parser_domain, hex_iterator
                                       , boost::variant<iiop::profile_body, profile_body_1_1_attr>, 0u
                                       > tagged_profile_body
    (giop::endianness[profile_body_1_0 | profile_body_1_1]);

  typedef fusion::vector2<std::string
                  , std::vector
                   <boost::variant<iiop::profile_body, profile_body_1_1_attr, ior::tagged_profile> >
                  > result_type;
  result_type r;

  typedef ior::grammar::ior<iiop::parser_domain, hex_iterator, result_type>
    ior_grammar_type;
  
  ior_grammar_type ior_grammar(tagged_profile_body | tagged_profile);
  namespace qi = boost::spirit::qi;
  Iterator first_tmp = first;
  if(qi::parse(first_tmp, last
               ,  qi::lit("IOR:")
               >> ior::hex
               [
                giop::compile<iiop::parser_domain>(giop::endianness[ior_grammar])
               ]
               , r))
  {
    std::cout << "Success" << std::endl;
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

    return sior;
  }

  throw std::runtime_error("invalid param");
}


namespace poa {

template <typename C, typename T>
struct object_registration
{
  typedef object_registration<C, T> self_type;
  typedef typename boost::remove_reference<T>::type value_type;
  struct orb orb_;
  T object;

  object_registration(struct orb orb, T object)
    : orb_(orb), object(object) {}

  template <int I, typename Enable = void>
  struct call_if_not_end;

  template <int I>
  inline void call_if(std::string const& method
                      , std::size_t align_offset, const char* first
                      , const char* last, bool little_endian, reply& r)
  {
    typedef typename C::requirements requirements;
    typedef typename mpl::at_c<requirements, I>::type method_concept;
    if(method == method_concept::name())
    {
      std::cout << "Should call method " << method << std::endl;
      handle_request_body
        <typename method_concept::result_type, typename method_concept::arguments>
        (orb_, &object, method_concept(), align_offset, first, last, little_endian, r);
    }
    else
      call_if_not_end<I+1>::do_(*this, method, align_offset, first, last, little_endian, r);
  }

  template <int I>
  struct call_if_not_end<I, typename boost::enable_if
                         <mpl::equal_to<mpl::size<typename C::requirements>, mpl::int_<I> > >::type>
  {
    inline static void do_(self_type& self, std::string const&, std::size_t, const char*, const char*, bool, reply&) {}
  };

  template <int I>
  struct call_if_not_end<I, typename boost::enable_if
                         <mpl::not_equal_to<mpl::size<typename C::requirements>, mpl::int_<I> > >::type>
  {
    inline static void do_(self_type& self, std::string const& method
                           , std::size_t align_offset, const char* first, const char* last
                           , bool little_endian, reply& r)
    {
      self.call_if<I>(method, align_offset, first, last, little_endian, r);
    }
  };

  void call(std::string const& method
            , std::size_t align_offset, const char* first
            , const char* last, bool little_endian
            , reply& r)
  {
    namespace fusion = boost::fusion;
    std::cout << "calling method " << method << std::endl;

    call_if_not_end<0>::do_(*this, method, align_offset, first, last, little_endian, r);
  }
};

}

}

#endif
