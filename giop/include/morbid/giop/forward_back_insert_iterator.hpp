/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_GIOP_FORWARD_BACK_INSERT_ITERATOR_HPP
#define MORBID_GIOP_FORWARD_BACK_INSERT_ITERATOR_HPP

#include <iterator>

namespace morbid { namespace giop {

template <typename Vector>
struct forward_back_insert_iterator
{
  typedef typename Vector::iterator base_iterator_type;
  typedef typename std::iterator_traits<base_iterator_type> base_iterator_traits;

  typedef typename base_iterator_traits::difference_type difference_type;
  typedef typename base_iterator_traits::value_type value_type;
  typedef typename base_iterator_traits::pointer pointer;
  typedef typename base_iterator_traits::reference reference;
  typedef typename std::random_access_iterator_tag iterator_category;

  Vector* c;
  mutable difference_type pos;

  typedef forward_back_insert_iterator<Vector> self_type;

  forward_back_insert_iterator(Vector& c)
    : c(&c), pos(c.size()) {}

  self_type& operator++()
  {
    ++pos; 
    return *this;
  }
  self_type& operator+=(int s)
  {
    pos += s;
    return *this;
  }
  self_type operator++(int)
  {
    self_type tmp(*this);
    ++*this;
    return tmp;
  }
  value_type& operator*() const
  {
    difference_type size = c->size();
    if(pos == size)
      c->push_back(value_type());
    return (*c)[pos];
  }
};

template <typename Container>
bool operator==(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return &lhs.c == &rhs.c && lhs.pos == rhs.pos;
}

template <typename Container>
bool operator!=(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return !(lhs == rhs);
}

template <typename Container>
bool operator<(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return lhs.pos < rhs.pos;
}

template <typename Container>
bool operator<=(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return lhs.pos <= rhs.pos;
}

template <typename Container>
bool operator>(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return lhs.pos > rhs.pos;
}

template <typename Container>
bool operator>=(forward_back_insert_iterator<Container> const& lhs, forward_back_insert_iterator<Container> const& rhs)
{
  return lhs.pos >= rhs.pos;
}

template <typename Container>
typename forward_back_insert_iterator<Container>::difference_type
operator-(forward_back_insert_iterator<Container> const& lhs
          , forward_back_insert_iterator<Container> const& rhs)
{
  return lhs.pos - rhs.pos;
}

template <typename Container>
forward_back_insert_iterator<Container> operator+(forward_back_insert_iterator<Container> lhs
                                                  , typename forward_back_insert_iterator<Container>::difference_type rhs)
{
  return lhs.pos += rhs;
}

template <typename Container>
forward_back_insert_iterator<Container> operator+(typename forward_back_insert_iterator<Container>::difference_type lhs
                                                  , forward_back_insert_iterator<Container> rhs)
{
  return rhs.pos += lhs;
}

} }

#endif
