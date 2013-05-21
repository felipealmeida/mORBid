/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IDL_COMPILER_COMPILATION_ERROR_HPP
#define MORBID_IDL_COMPILER_COMPILATION_ERROR_HPP

#include <boost/wave/util/file_position.hpp>

#include <boost/exception/all.hpp>

namespace morbid { namespace idl_compiler {

struct lookup_error : boost::exception
{
  lookup_error(idl_parser::types::scoped_name const& name)
    : name(name) {}
  ~lookup_error() throw() {}

  const char* what() const throw() { return "lookup_error"; }

  idl_parser::types::scoped_name name;
};

struct compilation_error : boost::exception
{
  compilation_error(const char* error_msg, boost::wave::util::file_position_type file_position)
    : error_msg_(error_msg), file_position_(file_position) {}
  ~compilation_error() throw() {}

  const char* what() const throw() { return "compilation_error"; }
  const char* error_message() const { return error_msg_; }
  boost::wave::util::file_position_type const& file_position() const { return file_position_; }

  const char* error_msg_;
  boost::wave::util::file_position_type file_position_;
};

} }

#endif
