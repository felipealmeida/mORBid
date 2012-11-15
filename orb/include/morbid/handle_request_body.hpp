/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

#ifndef TECORB_HANDLE_REQUEST_BODY_HPP
#define TECORB_HANDLE_REQUEST_BODY_HPP

#include <morbid/any.hpp>
#include <morbid/detail/max_args.hpp>
#include <morbid/parse_argument.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/serialize_object.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>

namespace morbid { namespace request_body_detail {

template <typename T, typename Enable = void>
struct wrapped_type;

template <typename T>
struct wrapped_type<T, typename boost::enable_if
                    <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                     , void>::type>
{
  typedef T type;
};                    

template <>
struct wrapped_type<String>
{
  typedef std::vector<char> type;
};

template <>
struct wrapped_type< ::morbid::Boolean>
{
  typedef bool type;
};

template <>
struct wrapped_type<Char>
{
  typedef Char type;
};

template <>
struct wrapped_type<Octet>
{
  typedef Octet type;
};

template <>
struct wrapped_type<Double>
{
  typedef Double type;
};

template <>
struct wrapped_type<Float>
{
  typedef Float type;
};

template <>
struct wrapped_type< ::morbid::Long>
{
  typedef ::morbid::Long type;
};

template <>
struct wrapped_type<Octet*>
{
  typedef std::vector<Octet> type;
};

template <>
struct wrapped_type< ::morbid::Short>
{
  typedef Short type;
};

template <>
struct wrapped_type<WChar>
{
  typedef WChar type;
};

template <>
struct wrapped_type<WChar*>
{
  typedef std::vector<WChar> type;
};

template <>
struct wrapped_type<morbid::Any_ptr>
{
  typedef morbid::Any_ptr type;
};

// template <>
// struct wrapped_type<morbid::Any_ptr>
// {
//   typedef morbid::Any_ptr type;
// };

template <typename T>
inline T const& unwrap(T const& o, typename boost::enable_if
                       <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                       , void*>::type = 0)
{
  return o;
}

template <typename T>
inline T& unwrap(T& o, typename boost::enable_if
                 <boost::is_same<typename T::_morbid_type_kind, struct_tag>
                 , void*>::type = 0)
{
  return o;
}

inline const char* unwrap(std::vector<char>const& v)
{
  return &v[0];
}

inline const unsigned char* unwrap(std::vector<unsigned char>const& v)
{
  return &v[0];
}

inline Boolean& unwrap(Boolean& v)
{
  return v;
}

inline Char& unwrap(Char& v)
{
  return v;
}

inline Octet& unwrap(Octet& v)
{
  return v;
}

inline Double& unwrap(Double& v)
{
  return v;
}

inline Float& unwrap(Float& v)
{
  return v;
}

inline Long& unwrap(Long& v)
{
  return v;
}

inline Short& unwrap(Short& v)
{
  return v;
}

inline WChar& unwrap(WChar& v)
{
  return v;
}

inline const WChar* unwrap(std::vector<WChar>const& v)
{
  return &v[0];
}

inline morbid::Any_ptr unwrap(morbid::Any_ptr v)
{
  return v;
}

template <typename Tag>
struct init_arg;

template <>
struct init_arg<type_tag::in_tag>
{
  template <typename R>
  static R call(const char* first, const char*& rq_current
                , const char* rq_last, bool little_endian)
  {
    std::cout << "init arg parsing in param " << typeid(R).name() << std::endl;
    return parse_argument(first, rq_current, rq_last, little_endian
                          , argument_tag<R>());
  }
};

template <>
struct init_arg<type_tag::out_tag>
{
  template <typename R>
  static R call(const char* first, const char*& rq_current
                , const char* rq_last, bool little_endian)
  {
    std::cout << "init arg default constructing out param " << typeid(R).name() << std::endl;
    return R();
  }
};

template <>
struct init_arg<type_tag::inout_tag> : init_arg<type_tag::in_tag>
{
};

template <typename Tag>
struct serialize_out_arg;

template <>
struct serialize_out_arg<type_tag::in_tag>
{
  template <typename OutputIterator, typename A>
  static void call(OutputIterator& iterator, bool b, A a)
  {
    std::cout << "NOT serialize out param " << typeid(A).name() << std::endl;
  }
};

template <>
struct serialize_out_arg<type_tag::out_tag>
{
  template <typename OutputIterator, typename A>
  static void call(OutputIterator& iterator, bool b, A a)
  {
    std::cout << "serialize out param " << typeid(A).name() << std::endl;
    serialization::serialize_object(iterator, a);
  }
};

template <>
struct serialize_out_arg<type_tag::inout_tag> : serialize_out_arg<type_tag::out_tag>
{
};

template <typename R, int>
struct make_request_and_call_function;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TECORB_MAX_ARGS \
                                         , "morbid/handle_request_body.hpp"))
#include BOOST_PP_ITERATE()

}

template <typename SeqParam, typename T, typename F>
void handle_request_body(T* self, F f, const char* first
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r)
{
  std::cout << "handle_request_body " << typeid(f).name() << std::endl;
  typedef typename boost::function_types::result_type<F>::type result_type;
  request_body_detail::make_request_and_call_function
    <result_type, boost::mpl::size<SeqParam>::value>
    ::template call<SeqParam>(self, f, first, rq_first, rq_last, little_endian, r);
}

}

#endif
#else

#define N() BOOST_PP_ITERATION()

#define MORBID_MAKE_REQUEST_original(I) BOOST_PP_CAT(A, I)
#define MORBID_MAKE_REQUEST_tag(I) BOOST_PP_CAT(BOOST_PP_CAT(A, I), _tag)
#define MORBID_MAKE_REQUEST_arg_type(I) BOOST_PP_CAT(BOOST_PP_CAT(a, I), _type)
#define MORBID_MAKE_REQUEST_arg_name(I) BOOST_PP_CAT(a, I)

#define MORBID_MAKE_REQUEST_ARGUMENT_INIT(Z, I, DATA)                   \
  typedef typename mpl::at_c<ParamSeq, I>::type::original_type          \
  MORBID_MAKE_REQUEST_original(I);                                      \
  typedef typename mpl::at_c<ParamSeq, I>::type::tag                    \
  MORBID_MAKE_REQUEST_tag(I);                                           \
  typedef typename wrapped_type<MORBID_MAKE_REQUEST_original(I)>::type  \
  MORBID_MAKE_REQUEST_arg_type(I);                                      \
  MORBID_MAKE_REQUEST_arg_type(I) MORBID_MAKE_REQUEST_arg_name(I)       \
  = init_arg<MORBID_MAKE_REQUEST_tag(I)>                           \
    ::template call<MORBID_MAKE_REQUEST_arg_type(I)> \
    (first, rq_current, rq_last, little_endian);

#define MORBID_MAKE_REQUEST_ARGUMENT_SERIALIZE_OUTPUT(Z, I, DATA)       \
  serialize_out_arg<MORBID_MAKE_REQUEST_tag(I)>                         \
  ::call(iterator, true, MORBID_MAKE_REQUEST_arg_name(I));

#define TECORB_HANDLE_REQUEST_unwrap(z, n, data) \
  BOOST_PP_COMMA_IF(n) unwrap( BOOST_PP_CAT(a, n) )

template <>
struct make_request_and_call_function<void, N()>
{
  template <typename ParamSeq, typename T, typename F>
  static void call(T* self, F f, const char* first, const char* rq_current
                   , const char* rq_last, bool little_endian, reply& r)
  {
    std::cout << "make_request_and_call_function" << std::endl;
    namespace mpl = boost::mpl;
    BOOST_PP_REPEAT(N(), MORBID_MAKE_REQUEST_ARGUMENT_INIT, ~)

    (self->*f)(BOOST_PP_REPEAT(N(), TECORB_HANDLE_REQUEST_unwrap, ~));

    std::back_insert_iterator<std::vector<char> > iterator(r.reply_body);
    BOOST_PP_REPEAT(N(), MORBID_MAKE_REQUEST_ARGUMENT_SERIALIZE_OUTPUT, ~)
  }
};

template <typename R>
struct make_request_and_call_function<R, N()>
{
  template <typename ParamSeq, typename T, typename F>
  static void call(T* self, F f, const char* first, const char* rq_current
                   , const char* rq_last, bool little_endian, reply& r)
  {
    std::cout << "make_request_and_call_function" << std::endl;
    namespace mpl = boost::mpl;
    BOOST_PP_REPEAT(N(), MORBID_MAKE_REQUEST_ARGUMENT_INIT, ~)

    R result = (self->*f)(BOOST_PP_REPEAT(N(), TECORB_HANDLE_REQUEST_unwrap, ~));

    std::back_insert_iterator<std::vector<char> > iterator(r.reply_body);
    BOOST_PP_REPEAT(N(), MORBID_MAKE_REQUEST_ARGUMENT_SERIALIZE_OUTPUT, ~)
    serialization::serialize_object(iterator, result);
  }
};

#undef MORBID_MAKE_REQUEST_original
#undef MORBID_MAKE_REQUEST_tag
#undef MORBID_MAKE_REQUEST_arg_type
#undef MORBID_MAKE_REQUEST_arg_name
#undef MORBID_MAKE_REQUEST_ARGUMENT_INIT
#undef TECORB_HANDLE_REQUEST_unwrap

// template <typename T, typename F BOOST_PP_ENUM_TRAILING_PARAMS(N(), typename A)>
// typename boost::function_types::result_type<F>::type
// handle_parameter (T* self, F f, const char* first, const char* rq_current
//                        , const char* rq_last, boost::mpl::true_
//                        , bool little_endian
//                        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N(), A, a))
// {
//   std::cout << "Finished, call" << std::endl;
//   return (self->*f)(BOOST_PP_REPEAT(N(), TECORB_HANDLE_REQUEST_unwrap, ~));
// }

// #undef TECORB_HANDLE_REQUEST_unwrap

// template <typename T, typename F BOOST_PP_ENUM_TRAILING_PARAMS(N(), typename A)>
// typename boost::function_types::result_type<F>::type
// handle_parameter (T* self, F f, const char* first, const char* rq_current
//                   , const char* rq_last, boost::mpl::false_
//                   , bool little_endian
//                   BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N(), A, a))
// {
//   std::cout << "Not finished, should read parameter and continue" << std::endl;
//   typedef typename boost::function_types::parameter_types<F>::type parameter_types;
//   typedef typename boost::mpl::deref
//     <typename boost::mpl::next
//      <typename boost::mpl::begin<parameter_types>::type>::type>::type
//     parameter_type;

//   std::cout << "parameter type: " << typeid(parameter_type).name() << std::endl;

//   typename wrapped_type<parameter_type>::type
//     r = parse_argument(first, rq_current, rq_last, little_endian
//                        , argument_tag<parameter_type>());
//   typedef boost::mpl::bool_<boost::mpl::size<parameter_types>::value == N()+2> stop_type;
//   return handle_parameter(self, f, first, rq_current, rq_last, stop_type(), little_endian
//                           BOOST_PP_ENUM_TRAILING_PARAMS(N(), a), r);
// }

#undef N
#endif
