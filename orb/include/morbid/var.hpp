/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_VAR_HPP
#define MORBID_VAR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace morbid {

template <typename T>
struct var
{
  var(boost::shared_ptr<T> p)
    : p(p) {}

  boost::shared_ptr<T> operator->() const { return p; }
  operator boost::shared_ptr<T>() const { return p; }

  boost::shared_ptr<T> in() const { return p; }
  bool operator!() const { return !p; }

private:
  boost::shared_ptr<T> p;
};

template <typename T>
struct array_var
{
  array_var(boost::shared_array<T> p)
    : p(p) {}

  T* in() const { return p.get(); }

private:
  boost::shared_array<T> p;
};

}

#endif
