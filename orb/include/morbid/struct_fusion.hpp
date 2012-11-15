/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_ORB_STRUCT_FUSION_HPP
#define MORBID_ORB_STRUCT_FUSION_HPP

#include <morbid/type_tag.hpp>

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/mpl/at.hpp>
#include <boost/fusion/mpl/back.hpp>
#include <boost/fusion/mpl/begin.hpp>
#include <boost/fusion/mpl/end.hpp>
#include <boost/fusion/mpl/size.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/at.hpp>

namespace morbid { namespace fusion_adapt {

namespace mpl = boost::mpl;

struct struct_sequence_tag;

template <typename Struct, std::size_t Pos>
struct struct_sequence_iterator
  : boost::fusion::iterator_base<struct_sequence_iterator<Struct, Pos> >
{
  typedef typename Struct::_morbid_fusion_size seq_size;
  typedef typename Struct::_morbid_fusion_types types_seq;

  // BOOST_MPL_ASSERT_RELATION(Pos, >=, 0)
  // BOOST_MPL_ASSERT_RELATION(Pos, <=, seq_size::value)

  typedef Struct struct_type;
  typedef boost::mpl::size_t<Pos> index;
  typedef boost::fusion::random_access_traversal_tag category;
  typedef struct_sequence_tag fusion_tag;

  typedef typename mpl::at_c<types_seq, Pos>::type mutable_value_type;
  typedef typename mpl::if_<boost::is_const<Struct>
                            , mutable_value_type const
                            , mutable_value_type>::type value_type;

  struct_sequence_iterator(Struct& struct_)
    : struct_(struct_) {}

  Struct& struct_;

  value_type& call() const
  {
    return struct_._morbid_fusion_at(index());
  }
};

} }

namespace boost { namespace fusion { namespace traits {

template <typename T>
struct tag_of<T, typename boost::enable_if
              <boost::is_same<typename T::_morbid_type_kind, ::morbid::struct_tag>
               , void>::type>
{
  typedef ::morbid::fusion_adapt::struct_sequence_tag type;
};

}

namespace extension {

template <>
struct value_of_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template <typename Iterator>
  struct apply
  {
    typedef typename Iterator::value_type type;
  };
};

template <>
struct deref_impl<  ::morbid::fusion_adapt::struct_sequence_tag>
{
  template <typename Iterator>
  struct apply
  {
    typedef typename boost::mpl::if_
    <typename boost::is_const<Iterator>::type
     , typename Iterator::value_type const&
     , typename Iterator::value_type&>::type type;
    // // typedef typename Iterator::value_type type;
    // typedef int type;

    static type call(Iterator const& it)
    {
      return it.call();
    }
  };
};

template <>
struct at_impl<  ::morbid::fusion_adapt::struct_sequence_tag>
{
  template <typename Sequence, typename I>
  struct apply
  {
    typedef ::morbid::fusion_adapt::struct_sequence_iterator<Sequence, I::value>
      iterator_type;
    typedef typename iterator_type::value_type type;

    static type& call(Sequence& s)
    {
      return s._morbid_fusion_at( ::boost::mpl::size_t<I::value>());
    }
  };
};

template<>
struct is_sequence_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename T>
  struct apply : mpl::true_ {};
};

template<>
struct is_view_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename T>
  struct apply : mpl::false_ {};
};

template<>
struct begin_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename Sequence>
  struct apply
  {
    typedef ::morbid::fusion_adapt::struct_sequence_iterator<Sequence, 0u> type;

    static type call(Sequence& seq)
    {
      return type(seq);
    }
  };
};

template<>
struct end_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename Sequence>
  struct apply
  {
    typedef ::morbid::fusion_adapt::struct_sequence_iterator<Sequence, Sequence::_morbid_fusion_size::value> type;

    static type call(Sequence& seq)
    {
      return type(seq);
    }
  };
};

template<>
struct size_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename Sequence>
  struct apply : Sequence::_morbid_fusion_size
  {
  };
};

template<>
struct next_impl< ::morbid::fusion_adapt::struct_sequence_tag>
{
  template<typename Iterator>
  struct apply
  {
    typedef typename Iterator::struct_type struct_type;
    typedef typename Iterator::index index;
    typedef ::morbid::fusion_adapt::struct_sequence_iterator<struct_type, index::value + 1> type;

    static type
    call(Iterator const& i)
    {
      return type(i.struct_);
    }
  };
};

} } }

#endif

