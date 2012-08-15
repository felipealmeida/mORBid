#ifndef TECORB_IDL_PARSER_INTERFACE_DEF_HPP
#define TECORB_IDL_PARSER_INTERFACE_DEF_HPP

#include <boost/range/iterator_range.hpp>

#include <ostream>

namespace tecorb { namespace idl_parser {

template <typename Iterator>
struct interface_def
{
  typedef typename Iterator::base_iterator_type base_iterator;
  interface_def() {}
  interface_def(boost::iterator_range<base_iterator> name)
    : name(name) {}

  boost::iterator_range<base_iterator> name;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, interface_def<Iterator> d)
{
  os << "[interface_def name: " << d.name << "]";
  return os;
}

} }

#endif
