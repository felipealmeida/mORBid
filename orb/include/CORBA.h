/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_CORBA_H
#define TECORB_CORBA_H

#include <morbid/any.hpp>
#include <morbid/string.hpp>
#include <morbid/orb.hpp>
#include <morbid/object.hpp>
#include <morbid/poa.hpp>
#include <morbid/exception.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/in_out_traits.hpp>

namespace CORBA {

using ::morbid::Any;
using ::morbid::Any_var;
using ::morbid::Any_ptr;

using ::morbid::MARSHALL;

using ::morbid::ORB;
using ::morbid::ORB_ptr;
using ::morbid::ORB_var;

using ::morbid::ORB_init;

using ::morbid::Object;
using ::morbid::Object_ptr;
using ::morbid::Object_var;

using ::morbid::is_nil;

using ::morbid::String;
using ::morbid::String_ptr;
using ::morbid::String_var;

typedef String ObjectId;
typedef String_ptr ObjectId_ptr;
typedef String_var ObjectId_var;

using ::morbid::Boolean;
using ::morbid::ULong;
using ::morbid::Long;
using ::morbid::ULongLong;
using ::morbid::LongLong;
using ::morbid::UShort;
using ::morbid::Short;
using ::morbid::Double;
using ::morbid::Float;
using ::morbid::Char;
using ::morbid::WChar;
using ::morbid::Octet;

using ::morbid::String_out;
using ::morbid::Boolean_out;
using ::morbid::UShort_out;
using ::morbid::Short_out;
using ::morbid::ULong_out;
using ::morbid::Long_out;
using ::morbid::ULongLong_out;
using ::morbid::LongLong_out;
using ::morbid::Double_out;
using ::morbid::Float_out;
using ::morbid::Char_out;
using ::morbid::WChar_out;
using ::morbid::Octet_out;

}

namespace PortableServer {

using ::morbid::poa::POA;
using ::morbid::poa::POA_var;
using ::morbid::poa::POA_ptr;

using ::morbid::poa::POAManager;
using ::morbid::poa::POAManager_var;
using ::morbid::poa::POAManager_ptr;

typedef CORBA::String ObjectId;
typedef CORBA::String_ptr ObjectId_ptr;
typedef CORBA::String_var ObjectId_var;

}

#endif
