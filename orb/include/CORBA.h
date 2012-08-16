/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_CORBA_H
#define TECORB_CORBA_H

#include <tecorb/string.hpp>
#include <tecorb/orb.hpp>
#include <tecorb/object.hpp>
#include <tecorb/poa.hpp>
#include <tecorb/exception.hpp>

namespace CORBA {

using ::tecorb::MARSHALL;

using ::tecorb::ORB;
using ::tecorb::ORB_ptr;
using ::tecorb::ORB_var;

using ::tecorb::ORB_init;

using ::tecorb::Object;
using ::tecorb::Object_ptr;
using ::tecorb::Object_var;

using ::tecorb::is_nil;

using ::tecorb::String;
using ::tecorb::String_ptr;
using ::tecorb::String_var;

typedef String ObjectId;
typedef String_ptr ObjectId_ptr;
typedef String_var ObjectId_var;

}

namespace PortableServer {

using ::tecorb::poa::POA;
using ::tecorb::poa::POA_var;
using ::tecorb::poa::POA_ptr;

using ::tecorb::poa::POAManager;
using ::tecorb::poa::POAManager_var;
using ::tecorb::poa::POAManager_ptr;

typedef CORBA::String ObjectId;
typedef CORBA::String_ptr ObjectId_ptr;
typedef CORBA::String_var ObjectId_var;

}

#endif
