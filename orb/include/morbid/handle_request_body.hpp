/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_HANDLE_REQUEST_BODY_HPP
#define MORBID_HANDLE_REQUEST_BODY_HPP

#include <morbid/any.hpp>
#include <morbid/detail/max_args.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/arguments_traits.hpp>
#include <morbid/giop/grammars/arguments.hpp>
#include <morbid/iiop/all.hpp>

#include <boost/fusion/include/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/copy_if.hpp>

namespace morbid {

namespace fusion = boost::fusion;

template <typename T>
struct create_parse_argument_transform
{
  typedef typename boost::remove_const<
    typename boost::remove_reference<typename T::original_type>::type>::type type;
};

template <typename Tag>
struct create_parse_argument_transform<type_tag::value_type_tag<morbid::string, Tag> >
{
  typedef std::string type;
};

template <typename T>
struct create_argument_transform;

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::in_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::in_tag> tagged;
  typedef typename boost::add_reference<typename tagged::type>::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::out_tag> >
{
  typedef type_tag::value_type_tag<T, type_tag::out_tag> tagged;
  typedef typename tagged::type type;
  typedef type result_type;

  result_type operator()(result_type r) const { return r; }
};

template <typename T>
struct create_argument_transform<type_tag::value_type_tag<T, type_tag::inout_tag> >
  : create_argument_transform<type_tag::value_type_tag<T, type_tag::in_tag> >
{
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
  typedef const char* type;
  typedef type result_type;

  result_type operator()(std::string const& str) const
  {
    return str.c_str();
  }
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::out_tag> >
  : create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
};

template <>
struct create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::inout_tag> >
  : create_argument_transform<type_tag::value_type_tag<morbid::string, type_tag::in_tag> >
{
};

template <typename T>
struct tag
{
};

template <typename ParseArguments>
struct initialize_arguments
{
  template <typename R>
  struct result;

  template <typename This, int I, typename S, typename T>
  struct result<This(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>
  {
    typedef type_tag::is_not_out_type_tag<T> is_not_out;
    typedef typename mpl::if_
    <is_not_out
     , std::pair<mpl::int_<I+1>, fusion::cons<typename create_argument_transform<T>::type, S> >
     , std::pair<mpl::int_<I>, fusion::cons<typename create_argument_transform<T>::type, S> >
    >::type
     type;
  };

  initialize_arguments(ParseArguments& parse_arguments)
    : parse_arguments(parse_arguments)
  {}

  template <int I, typename S, typename T>
  typename result<initialize_arguments<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S> const& s, tag<T>const&, type_tag::out_tag) const
  {
    typedef typename create_argument_transform<T>::type type;
    return std::pair<mpl::int_<I>, fusion::cons<type, S> >
      (mpl::int_<I>(), fusion::cons<type, S>(typename T::type(), s.second));
  }

  template <int I, typename S, typename T, typename Tag>
  typename result<initialize_arguments<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  call(std::pair<mpl::int_<I>, S> const& s, tag<T>const&, Tag) const
  {
    typedef create_argument_transform<T> transform;
    typedef typename create_argument_transform<T>::type type;
    return std::pair<mpl::int_<I+1>, fusion::cons<type, S> >
      (mpl::int_<I+1>(), fusion::cons<type, S>(transform()(fusion::at_c<I>(parse_arguments)), s.second));
  }

  template <int I, typename S, typename T>
  typename result<initialize_arguments<ParseArguments>(std::pair<mpl::int_<I>, S>const&, tag<T>const&)>::type
  operator()(std::pair<mpl::int_<I>, S> const& s, tag<T>const& t) const
  {
    return call(s, t, typename T::tag());
  }

  ParseArguments& parse_arguments;
};

template <typename SeqParam, typename T, typename F>
void handle_request_body(T* self, F f, const char* first
                         , const char* rq_first, const char* rq_last
                         , bool little_endian, reply& r)
{
  std::cout << "handle_request_body " << typeid(f).name() << std::endl;
  typedef typename boost::function_types::result_type<F>::type result_type;

  typedef typename mpl::lambda<type_tag::is_not_out_type_tag<mpl::_1> >::type is_not_out_lambda;
  typedef typename mpl::copy_if<SeqParam, is_not_out_lambda>::type not_out_params;

  typedef typename mpl::transform<not_out_params, create_parse_argument_transform<mpl::_1> >::type mpl_parse_argument_types;

  typedef typename fusion::result_of::as_vector<mpl_parse_argument_types>::type parse_argument_types;

  parse_argument_types parse_arguments;

  morbid::arguments_traits arguments_traits;
  typedef giop::grammars::arguments<iiop::parser_domain
                                    , const char*, not_out_params, parse_argument_types>
    arguments_grammar;
  arguments_grammar arguments_grammar_(arguments_traits);

  namespace qi = boost::spirit::qi;
  if(qi::parse(rq_first, rq_last, giop::compile<iiop::parser_domain>(arguments_grammar_(giop::little_endian))
               , parse_arguments))
  {
    std::cout << "Parsed arguments correctly" << std::endl;

    std::cout << "handle_request_body not_out_params " << typeid(parse_argument_types).name() << std::endl;

    typedef typename mpl::transform<SeqParam, create_argument_transform<mpl::_1> >::type mpl_argument_types;
    typedef typename mpl::transform<SeqParam, tag<mpl::_1> >::type mpl_identity_argument_types;

    typedef typename fusion::result_of::as_vector<mpl_identity_argument_types>::type identity_argument_types;
    identity_argument_types const identity_arguments;

    typedef typename boost::remove_reference<
      typename fusion::result_of::fold<identity_argument_types const, std::pair<mpl::int_<0>, fusion::nil>
                                       , initialize_arguments<parse_argument_types> >::type
      >::type
      argument_types;

    std::cout << "handle_request_body not_out_params " << typeid(identity_argument_types).name() << std::endl;
    std::cout << "handle_request_body not_out_params " << typeid(argument_types).name() << std::endl;

    argument_types arguments = fusion::fold(identity_arguments, std::pair<mpl::int_<0>, fusion::nil>()
                                            , initialize_arguments<parse_argument_types>(parse_arguments));

    fusion::fused<F> fused(f);
    fused(fusion::push_front(arguments.second, self));
  }
  else
  {
    throw morbid::MARSHALL();
  }
}

}

#endif
