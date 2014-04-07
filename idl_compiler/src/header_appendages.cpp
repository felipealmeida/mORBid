/* (c) Copyright 2014 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <morbid/idl_compiler/iterator.hpp>

#include <boost/spirit/home/karma.hpp>

#include <iterator>

namespace morbid { namespace idlc {

void generate_header_prefix(output_iterator iterator)
{
  namespace karma = boost::spirit::karma;
  bool r = karma::generate
    (iterator
     , karma::lit("// -*- c++ -*-") << karma::eol
     << "// Generated header. DO NOT EDIT" << karma::eol << karma::eol
     << "#include <morbid/handle_request_body.hpp>" << karma::eol
     << "#include <morbid/reply.hpp>" << karma::eol
     << "#include <morbid/structured_ior.hpp>" << karma::eol
     << "#include <morbid/in_out_traits.hpp>" << karma::eol << karma::eol
     << "#include <morbid/synchronous_call.hpp>" << karma::eol << karma::eol
     << "#include <morbid/orb.hpp>" << karma::eol << karma::eol
     << "#include <morbid/reference.hpp>" << karma::eol << karma::eol
     << "#include <morbid/object.hpp>" << karma::eol << karma::eol
     << "#include <morbid/lazy_eval.hpp>" << karma::eol << karma::eol
     << "#include <boost/integer.hpp>" << karma::eol
     << "#include <boost/spirit/home/karma.hpp>" << karma::eol
     << "#include <boost/fusion/include/vector10.hpp>" << karma::eol
     << "#include <boost/fusion/include/vector20.hpp>" << karma::eol
     << "#include <boost/fusion/include/vector30.hpp>" << karma::eol
     << "#include <boost/fusion/include/vector40.hpp>" << karma::eol
     << "#include <boost/fusion/include/vector50.hpp>" << karma::eol
     << "#include <boost/type_erasure/any.hpp>" << karma::eol
     << "#include <boost/type_erasure/member.hpp>" << karma::eol
     << "#include <boost/type_erasure/concept_interface.hpp>" << karma::eol
     << karma::eol
     );
  if(!r) 
    throw std::runtime_error("Failed generating #includes for header");
}

} }
