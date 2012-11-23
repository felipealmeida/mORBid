/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_IIOP_GENERATOR_HPP
#define MORBID_IIOP_IIOP_GENERATOR_HPP

#include <morbid/iiop/generator/integer.hpp>

#include <boost/mpl/for_each.hpp>

namespace morbid { namespace iiop {

namespace fusion = boost::fusion;
namespace proto = boost::proto;
namespace mpl = boost::mpl;
namespace spirit = boost::spirit;
namespace karma = boost::spirit::karma;

template <typename Domain, typename Iterator, typename T
          , typename Enabler = void>
struct primitive_traits_impl;

template <typename Iterator>
struct primitive_traits_impl<karma::domain, Iterator, bool, void>
{
  typedef spirit::terminal<spirit::tag::char_code
                           <spirit::tag::char_, spirit::char_encoding::standard> >
  expr_type;
  static expr_type const& expr(bool b) { return karma::char_; }
};

template <typename Iterator, typename T>
struct primitive_traits_impl<karma::domain, Iterator, T
                             , typename boost::enable_if
                             <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                              , void>::type>
{
  typedef spirit::terminal<spirit::tag::char_code
                           <spirit::tag::char_, spirit::char_encoding::standard> >
  expr_type;
  static expr_type const& expr(bool b) { return karma::char_; }
};

struct primitive_traits
{
  template <typename Domain, typename Iterator, typename T>
  struct impl : primitive_traits_impl<Domain, Iterator, T>
  {};
};

template <typename Domain, typename Iterator, typename T
          , typename Enabler = void>
struct grammar_traits_impl;

template <typename Iterator, typename T>
struct grammar_traits_impl<karma::domain, Iterator, T
                           , typename boost::enable_if
                           <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                            , void>::type>
{
  typedef typename T::template _morbid_generator<Iterator> expr_type;
  template <typename U>
  static expr_type const& expr(U const& u);
};

template <typename Iterator, typename T>
struct grammar_traits_impl<karma::domain, Iterator, T
                           , typename boost::disable_if
                           <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                            , void>::type>
{
  typedef boost::mpl::void_ expr_type;
  template <typename U>
  static expr_type expr(U const& u);
};

namespace detail {

template <typename Domain, typename Iterator, typename TypeTraits>
struct sequence_builder
{
  template <typename T>
  struct result;

  template <typename This, typename S, typename T>
  struct result<This(S, T)>
  {
    typedef fusion::cons
    <spirit::terminal<iiop::generator::tag::align>
     , fusion::cons<typename TypeTraits::template impl
                    <Domain, Iterator, typename T::original_type>::expr_type
                    , typename boost::remove_reference<S>::type> > type;
  };
};

template <typename Domain, typename Iterator, typename TypeTraits
          , typename S, typename T>
struct grammars_sequence_builder
{
  typedef fusion::cons
  <spirit::terminal<iiop::generator::tag::align>
   , fusion::cons<typename TypeTraits::template impl
                  <Domain, Iterator, typename T::original_type>::expr_type
                  , typename boost::remove_reference<S>::type> > type;
};

struct grammars_traits
{
  template <typename Domain, typename Iterator, typename T>
  struct impl : grammar_traits_impl<Domain, Iterator, T>
  {};
};

template <typename T>
struct untag
{
  typedef typename T::original_type type;
};

template <typename Domain, typename Iterator, typename Sequence, typename TypeTraits>
struct grammar_holder
{

  typedef typename mpl::transform
    <Sequence, untag<mpl::_> >::type original_sequence;

  typedef typename 
  mpl::fold<Sequence, boost::fusion::nil
            , detail::grammars_sequence_builder
            <Domain, Iterator, grammars_traits, mpl::_, mpl::_>
            >::type elements_type;
  grammar_holder()
  {
    std::cout << "tagged sequence " << typeid(original_sequence).name() << std::endl;
  }

  template <typename T>
  typename fusion::result_of::at_c
  <elements_type
   , mpl::distance<typename mpl::begin<original_sequence>::type
                   , typename mpl::find<original_sequence, T>::type>::value>
  ::type& get()
  {
    BOOST_MPL_ASSERT((mpl::contains<original_sequence, T>));
    typedef mpl::distance<typename mpl::begin<original_sequence>::type
                          , typename mpl::find<original_sequence, T>::type> index_type;
    return fusion::at<index_type>(elements);
  }

  typename boost::remove_reference<elements_type>::type elements;
};

}

template <typename T>
struct tag{};

template <typename Holder>
struct test_holder
{
  test_holder(Holder& holder) : holder(holder) {}

  template <typename T>
  void operator()(tag<T>) const
  {
    std::cout << "T: " << typeid(T).name() << std::endl;
    std::cout << " get " << typeid(holder
                                   .template get<T>()).name()
              << std::endl;
  }

  Holder& holder;
};

template <typename T>
struct do_tag
{
  typedef tag<T> type;
};

template <typename Domain, typename Iterator, typename Sequence, typename TypeTraits>
struct iiop_grammar_builder
{
  iiop_grammar_builder()
  {
    mpl::for_each<Sequence, do_tag<detail::untag<mpl::_> > >
      (test_holder<holder_type>(holder));
  }
  
  typedef detail::grammar_holder<Domain, Iterator, Sequence, TypeTraits> holder_type;
  holder_type holder;
  
};

} }

#endif
