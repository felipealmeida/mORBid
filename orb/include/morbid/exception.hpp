/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_EXCEPTION_HPP
#define TECORB_EXCEPTION_HPP

#include <stdexcept>

namespace morbid {

struct SystemException : virtual std::exception
{
  const char* what() const throw() { return "SystemException"; };
};

struct UserException : virtual std::exception
{
  const char* what() const throw() { return "SystemException"; };
};

struct MARSHALL : SystemException
{
  const char* what() const throw() { return "MARSHALL"; };
};

struct INVALID_PARAM : SystemException
{
  const char* what() const throw() { return "INVALID_PARAM"; };
};

}

#endif
