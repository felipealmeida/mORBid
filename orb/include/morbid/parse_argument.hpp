/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_PARSE_ARGUMENT_HPP
#define MORBID_PARSE_ARGUMENT_HPP

#include <morbid/type_tag.hpp>
#include <morbid/iiop/grammar/sequence.hpp>
#include <morbid/iiop/grammar/align.hpp>
#include <morbid/iiop/grammar/types/primitives.hpp>
#include <morbid/any.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/exception.hpp>
#include <morbid/reference_wrapper.hpp>
#include <morbid/ior/grammar/ior.hpp>
#include <morbid/iiop/grammar/profile_body_1_1.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/spirit/home/qi.hpp>

#include <limits>
#include <iomanip>

namespace morbid {

namespace qi = boost::spirit::qi;

template <typename T>
struct argument_tag
{
};

inline Boolean parse_argument(const char* first, const char*& rq_current
                              , const char* rq_last, bool little_endian
                              , argument_tag<Boolean>)
{
  typedef iiop::grammar::boolean<const char*> boolean_grammar;
  boolean_grammar boolean;
  bool r = 0;
  if(boost::spirit::qi::parse(rq_current, rq_last, boolean, r))
  {
    return r;
  }
  else
    throw std::runtime_error("Format error");
}

inline Char parse_argument(const char* first, const char*& rq_current
                           , const char* rq_last, bool little_endian
                           , argument_tag<Char>)
{
  if(rq_current != rq_last)
  {
    return *rq_current++;
  }
  else
    throw std::runtime_error("Format error");
}

inline Octet parse_argument(const char* first, const char*& rq_current
                            , const char* rq_last, bool little_endian
                            , argument_tag<Octet>)
{
  if(rq_current != rq_last)
  {
    return *rq_current++;
  }
  else
    throw std::runtime_error("Format error");
}

inline Double parse_argument(const char* first, const char*& rq_current
                             , const char* rq_last, bool little_endian
                             , argument_tag< ::morbid::Double>)
{
  BOOST_MPL_ASSERT_RELATION( (std::numeric_limits< ::morbid::Double>::is_iec559), ==, true);
  BOOST_MPL_ASSERT_RELATION( sizeof(::morbid::Double), ==, 8u);
  int const size = sizeof(::morbid::Double);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Double r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Double));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Double), &r);
    }
    rq_current += sizeof(::morbid::Double);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline Float parse_argument(const char* first, const char*& rq_current
                            , const char* rq_last, bool little_endian
                            , argument_tag< ::morbid::Float>)
{
  BOOST_MPL_ASSERT_RELATION( (std::numeric_limits< ::morbid::Float>::is_iec559), ==, true);
  BOOST_MPL_ASSERT_RELATION( sizeof(::morbid::Float), ==, 4u);
  int const size = sizeof(::morbid::Float);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Float r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Float));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Float), &r);
    }
    rq_current += sizeof(::morbid::Float);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline ::morbid::ULong parse_argument(const char* first, const char*& rq_current
                                      , const char* rq_last, bool little_endian
                                      , argument_tag< ::morbid::ULong>)
{
  int const size = sizeof(::morbid::ULong);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::ULong r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::ULong));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::ULong), &r);
    }
    rq_current += sizeof(::morbid::ULong);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline ::morbid::Long parse_argument(const char* first, const char*& rq_current
                                     , const char* rq_last, bool little_endian
                                     , argument_tag< ::morbid::Long>)
{
  int const size = sizeof(::morbid::Long);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Long r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Long));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Long), &r);
    }
    rq_current += sizeof(::morbid::Long);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline ::morbid::Short parse_argument(const char* first, const char*& rq_current
                                      , const char* rq_last, bool little_endian
                                      , argument_tag< ::morbid::Short>)
{
  int const size = sizeof(::morbid::Short);
  if(std::distance(rq_current, rq_last) >= size)
  {
    ::morbid::Short r;
    if(little_endian && true /* am I little endian? */)
    {
      std::memcpy(&r, rq_current, sizeof(::morbid::Short));
    }
    else // endianness doesnt match
    {
      std::reverse_copy(rq_current, rq_current + sizeof(::morbid::Short), &r);
    }
    rq_current += sizeof(::morbid::Short);
    return r;
  }
  else
    throw std::runtime_error("Error parsing");
}

inline WChar parse_argument(const char* first, const char*& rq_current
                            , const char* rq_last, bool little_endian
                            , argument_tag<WChar>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

template <typename T>
inline T parse_argument(const char* first, const char*& rq_current
                        , const char* rq_last, bool little_endian
                        , argument_tag<T>
                        , typename boost::enable_if
                        <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                        , void*>::type = 0)
{
  typename T::template _morbid_parser<const char*> parser;
  T obj;
  if(qi::parse(rq_current, rq_last, parser, obj))
    return obj;
  else
    throw MARSHALL();
}

template <typename T>
inline reference_wrapper<T> parse_argument(const char* first, const char*& rq_current
                                           , const char* rq_last, bool little_endian
                                           , argument_tag<morbid::reference_wrapper<T> >
                                           , typename boost::enable_if
                                           <boost::is_same<typename T::_morbid_type_kind, interface_tag>
                                           , void*>::type = 0)
{
  std::cout << "Parsing IOR dist" << std::distance(rq_current, rq_last) << std::endl;
  ior::grammar::ior<const char*> ior_grammar;
  ior::ior ior_;
  using boost::phoenix::val;
  if(boost::spirit::qi::parse(rq_current, rq_last, ior_grammar(val(first)), ior_))
  {
    std::cout << "Parsed IOR successfully" << std::endl;
    iiop::grammar::profile_body_1_1<std::vector<char>::const_iterator> profile_body_grammar;
    bool found = false;
    morbid::iiop::profile_body profile_body;
    for(std::vector<ior::tagged_profile>::const_iterator
          first = ior_.profiles.begin()
          , last = ior_.profiles.end()
          ;first != last; ++first)
    {
      if(first->tag == 0 /*TAG_INTERNET_IOP*/)
      {
        if(boost::spirit::qi::parse(first->profile_data.begin()
                                    , first->profile_data.end()
                                    , profile_body_grammar(val(first->profile_data.begin())
                                                           , little_endian)
                                    , profile_body))
        {
          std::cout << "Parsed IIOP profile body information host: " << profile_body.host
                    << " port: " << profile_body.port 
                    << " object_key:  " << boost::make_iterator_range(profile_body.object_key.begin()
                                                                      , profile_body.object_key.end()) << std::endl;
          found = true;
          break;
        }
        else
        {
          std::cout << "Couldn't parse IIOP tagged profile" << std::endl;
        }
      }
    }
    if(found)
    {
      structured_ior ior = {ior_.type_id};
      ior.structured_profiles.push_back(profile_body);
      Object_ptr p(new ::morbid::remote_stub::Object(ior));
      typename T::_ptr_type ptr = T::_narrow(p);
      reference_wrapper<T> w;
      w.ptr = ptr;
      w.ior = ior;
      return w;
    }
    else
      throw morbid::MARSHALL();
  }
  else
    throw MARSHALL();
}

inline std::vector<WChar> parse_argument(const char* first, const char*& rq_current
                                         , const char* rq_last, bool little_endian
                                         , argument_tag<std::vector<WChar> >)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

inline morbid::Any_ptr parse_argument(const char* first, const char*& rq_current
                                      , const char* rq_last, bool little_endian
                                      , argument_tag<morbid::Any_ptr>)
{
  throw std::runtime_error("NOT_IMPLEMENTED");
}

inline
std::vector<char> parse_argument(const char* first, const char*& rq_current
                                 , const char* rq_last, bool little_endian
                                 , argument_tag<std::vector<char> >)
{
  {
    std::cout << "start of argument reading" << std::endl;
  const char *iterator = rq_current
    , *last = rq_last;
  while(iterator != last)
  {
    std::cout.fill('0');
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::cout << std::hex << std::setw(2) << (unsigned int)(unsigned char)*iterator << " ";
    if(++iterator == last) break;
    std::endl(std::cout);
  }
  std::endl(std::cout);
  std::dec(std::cout);
  std::cout << "end of argument reading" << std::endl;
  }

  namespace qi = boost::spirit::qi;
  iiop::grammar::sequence<const char*, char> sequence_grammar;
  std::vector<char> sequence;
  if(qi::parse(rq_current, rq_last
               , iiop::grammar::align(first, 4u) >> sequence_grammar(little_endian)
               , sequence))
  {
    std::cout << "Parsed sequence" << std::endl;
    if(!sequence.empty() && !*boost::prior(sequence.end()))
      return sequence;
  }
  throw std::runtime_error("Error parsing argument");
}

inline std::vector<Octet>
parse_argument(const char* first, const char*& rq_current
               , const char* rq_last, bool little_endian
               , argument_tag<std::vector<Octet> >)
{
  std::vector<char> v = parse_argument(first, rq_current
                                       , rq_last, little_endian
                                       , argument_tag<std::vector<char> >());
  std::vector<unsigned char> r(v.begin(), v.end());
  return r;
}

}

#endif
