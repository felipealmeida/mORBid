/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef TECORB_POA_HPP
#define TECORB_POA_HPP

#include <tecorb/var.hpp>
#include <tecorb/narrow.hpp>
#include <tecorb/object.hpp>
#include <tecorb/string.hpp>

#include <boost/mpl/vector.hpp>

namespace tecorb { namespace poa {

struct ServantBase
{
  Object_ptr _this();
};

struct POAManager : narrow<POAManager, boost::mpl::vector1<LocalObject> >
{
  void activate();
};

typedef boost::shared_ptr<POAManager> POAManager_ptr;
typedef tecorb::var<POAManager> POAManager_var;

struct POA : narrow<POA, boost::mpl::vector1<LocalObject> >
{
  String_ptr activate_object(ServantBase*);
  Object_ptr id_to_reference(String_ptr);

  POAManager_ptr the_POAManager();
};

typedef boost::shared_ptr<POA> POA_ptr;
typedef tecorb::var<POA> POA_var;


} }

#endif
