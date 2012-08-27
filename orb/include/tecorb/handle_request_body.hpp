/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef TECORB_HANDLE_REQUEST_BODY_HPP
#define TECORB_HANDLE_REQUEST_BODY_HPP

#include <tecorb/any.hpp>
#include <tecorb/detail/max_args.hpp>
#include <tecorb/parse_argument.hpp>
#include <tecorb/primitive_types.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>

namespace tecorb { namespace request_body_detail {

template <typename T>
struct wrapped_type;

template <>
struct wrapped_type<const char*>
{
  typedef std::vector<char> type;
};

template <>
struct wrapped_type< ::tecorb::Boolean>
{
  typedef bool type;
};

template <>
struct wrapped_type<char>
{
  typedef char type;
};

template <>
struct wrapped_type<double>
{
  typedef double type;
};

template <>
struct wrapped_type<float>
{
  typedef float type;
};

template <>
struct wrapped_type< ::tecorb::Long>
{
  typedef long type;
};

template <>
struct wrapped_type<const unsigned char*>
{
  typedef std::vector<unsigned char> type;
};

template <>
struct wrapped_type< ::tecorb::Short>
{
  typedef short type;
};

template <>
struct wrapped_type<wchar_t>
{
  typedef wchar_t type;
};

template <>
struct wrapped_type<const wchar_t*>
{
  typedef std::vector<wchar_t> type;
};

template <>
struct wrapped_type<const tecorb::Any_ptr>
{
  typedef tecorb::Any_ptr type;
};

template <>
struct wrapped_type<tecorb::Any_ptr>
{
  typedef tecorb::Any_ptr type;
};

inline const char* unwrap(std::vector<char>const& v)
{
  return &v[0];
}

inline const unsigned char* unwrap(std::vector<unsigned char>const& v)
{
  return &v[0];
}

inline bool unwrap(bool v)
{
  return v;
}

inline char unwrap(char v)
{
  return v;
}

inline double unwrap(double v)
{
  return v;
}

inline float unwrap(float v)
{
  return v;
}

inline long unwrap(long v)
{
  return v;
}

inline short unwrap(short v)
{
  return v;
}

inline wchar_t unwrap(wchar_t v)
{
  return v;
}

inline const wchar_t* unwrap(std::vector<wchar_t>const& v)
{
  return &v[0];
}

inline tecorb::Any_ptr unwrap(tecorb::Any_ptr v)
{
  return v;
}

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TECORB_MAX_ARGS \
                                         , "tecorb/handle_request_body.hpp"))
#include BOOST_PP_ITERATE()

template <typename T, typename F>
void handle_request_body(T* self, F f, const char* first
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r
                         , boost::mpl::true_)
{
  std::cout << "handle_request_body " << typeid(f).name() << std::endl;
  typedef typename boost::function_types::parameter_types<F>::type parameter_types;
  typedef boost::mpl::bool_<boost::mpl::size<parameter_types>::value == 1> stop_type;
  request_body_detail::handle_parameter
    (self, f, first, rq_first, rq_last, stop_type(), little_endian);
}

inline void handle_return_reply(bool b, reply& r)
{
  r.reply_body.push_back(b);
}

template <typename T, typename F>
void handle_request_body(T* self, F f, const char* first
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r
                         , boost::mpl::false_)
{
  std::cout << "handle_request_body " << typeid(f).name() << std::endl;
  typedef typename boost::function_types::parameter_types<F>::type parameter_types;
  typedef boost::mpl::bool_<boost::mpl::size<parameter_types>::value == 1> stop_type;
  handle_return_reply
    (request_body_detail::handle_parameter
     (self, f, first, rq_first, rq_last, stop_type(), little_endian)
     , r);
}

}

template <typename T, typename F>
void handle_request_body(T* self, F f, const char* first
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r)
{
  std::cout << "handle_request_body " << typeid(f).name() << std::endl;
  typedef typename boost::function_types::result_type<F>::type result_type;
  request_body_detail::handle_request_body
    (self, f, first, rq_first, rq_last, little_endian, r
     , boost::mpl::bool_<boost::is_same<result_type, void>::value>());
}

}

#endif
#else

#define N() BOOST_PP_SUB(TECORB_MAX_ARGS, BOOST_PP_ITERATION())

#define TECORB_HANDLE_REQUEST_unwrap(z, n, data) \
  BOOST_PP_COMMA_IF(n) unwrap( BOOST_PP_CAT(a, n) )

template <typename T, typename F BOOST_PP_ENUM_TRAILING_PARAMS(N(), typename A)>
typename boost::function_types::result_type<F>::type
handle_parameter (T* self, F f, const char* first, const char* rq_current
                       , const char* rq_last, boost::mpl::true_
                       , bool little_endian
                       BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N(), A, a))
{
  std::cout << "Finished, call" << std::endl;
  return (self->*f)(BOOST_PP_REPEAT(N(), TECORB_HANDLE_REQUEST_unwrap, ~));
}

#undef TECORB_HANDLE_REQUEST_unwrap

template <typename T, typename F BOOST_PP_ENUM_TRAILING_PARAMS(N(), typename A)>
typename boost::function_types::result_type<F>::type
handle_parameter (T* self, F f, const char* first, const char* rq_current
                  , const char* rq_last, boost::mpl::false_
                  , bool little_endian
                  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N(), A, a))
{
  std::cout << "Not finished, should read parameter and continue" << std::endl;
  typedef typename boost::function_types::parameter_types<F>::type parameter_types;
  typedef typename boost::mpl::deref
    <typename boost::mpl::next
     <typename boost::mpl::begin<parameter_types>::type>::type>::type
    parameter_type;

  std::cout << "parameter type: " << typeid(parameter_type).name() << std::endl;

  typename wrapped_type<parameter_type>::type
    r = parse_argument(first, rq_current, rq_last, little_endian
                       , argument_tag<parameter_type>());
  typedef boost::mpl::bool_<boost::mpl::size<parameter_types>::value == N()+2> stop_type;
  return handle_parameter(self, f, first, rq_current, rq_last, stop_type(), little_endian
                          BOOST_PP_ENUM_TRAILING_PARAMS(N(), a), r);
}

#undef N
#endif
