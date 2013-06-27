/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_OBJECT_REGISTRATION_HPP
#define MORBID_ORB_OBJECT_REGISTRATION_HPP

#include <morbid/orb.hpp>
#include <morbid/detail/is_a.hpp>

namespace morbid {

namespace poa {

template <typename C, typename T>
struct object_registration
{
  typedef object_registration<C, T> self_type;
  typedef typename boost::remove_reference<T>::type value_type;
  boost::optional<struct orb> orb_;
  T object;

  object_registration(struct orb orb, T object)
    : orb_(orb), object(object) {}
  object_registration(T object)
    : object(object) {}

  template <int I, typename Enable = void>
  struct call_if_not_end;

  template <int I>
  inline void call_if(std::string const& method
                      , std::size_t align_offset, const char*& first
                      , const char* last, bool little_endian, reply& r)
  {
    typedef typename detail::requirements<C>::type requirements;
    typedef typename mpl::at_c<requirements, I>::type method_concept;
    std::cout << "Searching method " << method << " testing with "
              << method_concept::name()
              << " in concept " << typeid(C).name()
              << " in object " << typeid(T).name() << std::endl;
    if(method == method_concept::name())
    {
      std::cout << "Should call method " << method << std::endl;
      handle_request_body
        <typename method_concept::result_type, typename method_concept::arguments>
        (*orb_, &object, method_concept(), align_offset, first, last, little_endian, r);
    }
    else
      call_if_not_end<I+1>::do_(*this, method, align_offset, first, last, little_endian, r);
  }

  template <int I>
  struct call_if_not_end<I, typename boost::enable_if
                         <mpl::equal_to<mpl::size<typename detail::requirements<C>::type>, mpl::int_<I> > >::type>
  {
    inline static void do_(self_type& self, std::string const& method, std::size_t align_offset, const char*& arg_first
                           , const char* arg_last, bool little_endian, reply& r)
    {
      // function not found
      if(method == "_is_a")
      {
        namespace qi = boost::spirit::qi;
        std::string type_id;
        bool p = qi::parse(arg_first, arg_last
                           , giop::compile<iiop::parser_domain>
                           (
                            iiop::aligned(align_offset)
                            [
                             giop::endianness(giop::endian(little_endian))

                             [
                              giop::string
                             ]
                            ])
                           , type_id);
        assert(!!p);

        std::cout << "is_a type_id " << type_id << std::endl;

        bool res = detail::is_a<C>(type_id);
        type_tag::value_type_tag<bool, type_tag::in_tag> vres(res);
        boost::fusion::vector1<bool const&> v(res);
        make_request_reply<mpl::vector1<type_tag::value_type_tag<bool, type_tag::out_tag> > >(*self.orb_, r, v);
      }
      else if(method == "_non_existent")
      {
        boost::fusion::vector1<bool> v(false);
        make_request_reply<mpl::vector1<type_tag::value_type_tag<bool, type_tag::out_tag> > >(*self.orb_, r, v);
      }
      else
      {
        std::cout << "Searching method " << method
                  << " failed in concept " << typeid(C).name()
                  << " in object " << typeid(T).name() << std::endl;
        std::abort();
      }
    }
  };

  template <int I>
  struct call_if_not_end<I, typename boost::enable_if
                         <mpl::not_equal_to<mpl::size<typename detail::requirements<C>::type>, mpl::int_<I> > >::type>
  {
    inline static void do_(self_type& self, std::string const& method
                           , std::size_t align_offset, const char*& first, const char* last
                           , bool little_endian, reply& r)
    {
      self.call_if<I>(method, align_offset, first, last, little_endian, r);
    }
  };

  void call(std::string const& method
            , std::size_t align_offset, const char*& first
            , const char* last, bool little_endian
            , reply& r)
  {
    namespace fusion = boost::fusion;
    std::cout << "calling method " << method << std::endl;

    call_if_not_end<0>::do_(*this, method, align_offset, first, last, little_endian, r);
  }
  void set_orb(struct orb orb)
  {
    orb_ = orb;
  }

  const char* type_id() const
  {
    return C::type_id();
  }
};

}

template <typename C>
poa::object_registration_any& reference<C>::_object_registration_ref(struct orb orb)
{
  _object_registration.set_orb(orb);
  return _object_registration;
}

template <typename C>
poa::object_registration_any reference<C>::_object_registration_copy(struct orb orb)
{
  _object_registration.set_orb(orb);
  return _object_registration;
}

}

#endif
