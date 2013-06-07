/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_HANDLE_REQUEST_BODY_HPP
#define MORBID_HANDLE_REQUEST_BODY_HPP

#include <morbid/detail/max_args.hpp>
#include <morbid/primitive_types.hpp>
#include <morbid/type_tag.hpp>
#include <morbid/in_out_traits.hpp>
#include <morbid/arguments_traits.hpp>
#include <morbid/giop/grammars/arguments.hpp>
#include <morbid/giop/grammars/message_1_0.hpp>
#include <morbid/giop/grammars/reply_1_0.hpp>
#include <morbid/iiop/all.hpp>
#include <morbid/reply.hpp>

#include <boost/fusion/include/fused.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/advance.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/fusion/iterator/mpl.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/single_view.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy.hpp>

#include <boost/type_traits/is_fundamental.hpp>

namespace morbid {

namespace fusion = boost::fusion;

template <typename T>
struct tag
{
  typedef T untagged;
};

template <typename NotInParams, typename ReplyArguments>
void make_request_reply(struct orb orb, reply& r, ReplyArguments& reply_arguments)
{
  // std::cout << "Generating reply with reply arguments " << typeid(ReplyArguments).name() << std::endl;
  typedef ReplyArguments reply_argument_types;

  typedef giop::forward_back_insert_iterator<std::vector<char> > output_iterator_type;
  typedef std::vector<fusion::vector2<unsigned int, std::vector<char> > > service_context_list;
  typedef fusion::vector4<service_context_list
                          , unsigned int, unsigned int
                          , reply_argument_types>
    reply_attribute_type;
  typedef fusion::vector1<fusion::vector1<reply_attribute_type> >
    message_reply_attribute_type;

  typedef giop::grammars::arguments<iiop::generator_domain
                                    , output_iterator_type, NotInParams
                                    , reply_argument_types>
    arguments_grammar;
  typedef giop::grammars::reply_1_0<iiop::generator_domain
                                    , output_iterator_type, reply_attribute_type>
    reply_grammar;
  typedef giop::grammars::message_1_0<iiop::generator_domain
                                      , output_iterator_type, message_reply_attribute_type
                                      , 1u /* reply */>
    message_reply_grammar;
  
  morbid::arguments_traits arguments_traits(orb);
  arguments_grammar arguments_grammar_(arguments_traits);
  reply_grammar reply_grammar_(arguments_grammar_);
  message_reply_grammar message_grammar_(reply_grammar_);

  service_context_list l;
  message_reply_attribute_type message_attribute
    (fusion::make_vector
     (reply_attribute_type
      (l
       , r.request_id
       , 0u /* NO_EXCEPTION */
       , reply_arguments)));
  output_iterator_type iterator(r.reply_body);
  namespace karma = boost::spirit::karma;
  if(karma::generate(iterator, giop::compile<iiop::generator_domain>
                     (message_grammar_(giop::native_endian))
                     , message_attribute))
  {
    // std::cout << "reply generated" << std::endl;
  }
  else
  {
    std::cout << "Failed generating reply" << std::endl;
  }
}

template <typename Seq, typename Outs, typename Args>
struct split_arguments_sequence
{
  Args& args;
  split_arguments_sequence(Args& args) : args(args) {}

  template <typename T>
  struct result;

  template <typename S, typename Iter>
  struct result<split_arguments_sequence(S const&, Iter&)>
  {
    typedef typename boost::remove_reference<typename fusion::result_of::deref<Iter>::type>
    ::type::untagged deref_type;
    typedef typename type_tag::is_not_in_type_tag<deref_type>::type is_not_in;
    typedef typename type_tag::value_type<deref_type>::type value_type;
    typedef typename mpl::if_
    <is_not_in
     // , fusion::cons
     //   <typename boost::add_reference<typename boost::add_const<value_type>::type>::type
     //    , S>
     , typename fusion::result_of::push_back
     <
         S const
         , typename boost::add_reference<typename boost::add_const<value_type>::type>::type
     >::type
     , S
    >::type type;
  };

  template <typename S, typename Iter>
  typename result<split_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i, mpl::false_) const
  {
    return s;
  }

  template <typename S, typename Iter>
  typename result<split_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i, mpl::true_) const
  {
    typedef result<split_arguments_sequence(S const&, Iter&)> result_;
    typedef typename boost::remove_const<Iter>::type iterator;
    typedef typename mpl::begin<Seq const>::type begin_iterator;
    typedef mpl::iterator_range<begin_iterator, mpl::fusion_iterator<iterator> > before_range;
    typedef typename mpl::count_if<before_range, type_tag::is_not_in_type_tag<mpl::_1> >::type not_ins;
    // std::cout << "is NOT in, adding reference from generated args. Type: "
    //           << typeid(typename result_::value_type).name()
    //           << " not_ins " << not_ins::value << std::endl;
    // return typename result_::type(fusion::at<not_ins>(args), s);
    return fusion::push_back
      <S, typename fusion::result_of::at<Args, not_ins>::type&>
      (s, fusion::at<not_ins>(args));
  }

  template <typename S, typename Iter>
  typename result<split_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i) const
  {
    typedef result<split_arguments_sequence(S const&, Iter&)> result_;
    typedef typename result_::is_not_in is_not_in;
    return (*this)(s, i, is_not_in());
  }
};

template <typename R, typename SeqParam, typename F, typename T, typename Args>
void handle_request_reply(struct orb orb, F f, T* self, reply& r, Args& args, mpl::identity<void>)
{
  // std::cout << "handle_request_reply void return" << std::endl;
  fusion::single_view<T*> self_view(self);
  // joint_view requires lvalues
  fusion::joint_view<fusion::single_view<T*> const, Args> new_args(self_view, args);
  f(new_args);

  typedef typename mpl::copy_if<SeqParam, type_tag::is_not_in_type_tag<mpl::_1> >::type not_in_params;

  typedef typename mpl::transform
    <SeqParam
     , tag<mpl::_1> >::type tagged_arguments;
  typedef typename fusion::result_of::as_vector<tagged_arguments>::type fusion_tagged_arguments;
  fusion_tagged_arguments const fusion_tagged_arguments_;

  typedef split_arguments_sequence
    <fusion_tagged_arguments
     , not_in_params
     , Args> split_arguments;

  typedef typename boost::remove_reference
    <typename fusion::result_of::iter_fold
     <fusion_tagged_arguments const
      , fusion::vector0<>
      , split_arguments>::type>::type reply_argument_types;
  reply_argument_types reply_arguments = fusion::iter_fold(fusion_tagged_arguments_
                                                           , fusion::vector0<>()
                                                           , split_arguments(args));
  typedef typename fusion::result_of::as_vector<reply_argument_types>::type
    reply_argument_vector_type;
  reply_argument_vector_type reply_argument_vector
    = fusion::as_vector(reply_arguments);
  
  // std::cout << "(no result) reply_argument_types " << typeid(reply_argument_types).name() << std::endl;
  make_request_reply<not_in_params>(orb, r, reply_argument_vector);
}

template <typename R, typename SeqParam, typename F, typename T, typename Args>
void handle_request_reply(struct orb orb, F f, T* self, reply& r, Args& args, mpl::identity<R>)
{
  fusion::single_view<T*> self_view(self);
  // joint_view requires lvalues
  fusion::joint_view<fusion::single_view<T*> const, Args> new_args(self_view, args);
  typename return_traits<R>::type result = f(new_args);

  typedef typename mpl::copy_if<SeqParam, type_tag::is_not_in_type_tag<mpl::_1> >::type not_in_params;

  typedef typename mpl::transform
    <SeqParam
     , tag<mpl::_1> >::type tagged_arguments;
  typedef typename fusion::result_of::as_vector<tagged_arguments>::type fusion_tagged_arguments;
  fusion_tagged_arguments const fusion_tagged_arguments_;

  typedef split_arguments_sequence
    <fusion_tagged_arguments
     , not_in_params
     , Args> split_arguments;

  typedef typename boost::remove_reference
    <typename fusion::result_of::iter_fold
     <fusion_tagged_arguments const
      , fusion::vector0<>
      , split_arguments>::type>::type reply_argument_types;
  reply_argument_types reply_arguments = fusion::iter_fold(fusion_tagged_arguments_
                                                           , fusion::vector0<>()
                                                           , split_arguments(args));
  typedef typename fusion::result_of::as_vector
    <typename fusion::result_of::push_front
     <reply_argument_types
      , typename boost::add_reference
        <
          typename boost::add_const<typename return_traits<R>::type>::type
        >::type
     >::type
    >::type args_with_result_type;

  args_with_result_type args_with_result
    = fusion::as_vector(fusion::push_front(reply_arguments, boost::ref(result)));
  typedef mpl::equal_to
    <typename mpl::next<typename mpl::size<reply_argument_types>::type>::type
     , typename mpl::size<args_with_result_type>::type
    > condition;
  BOOST_MPL_ASSERT((condition));
  typedef typename mpl::push_front
    <
      not_in_params
    , type_tag::value_type_tag<R, type_tag::out_tag>
    >::type reply_params;
  make_request_reply<reply_params>(orb, r, args_with_result);
}

template <typename Seq, typename Outs, typename Args>
struct merge_arguments_sequence
{
  Args& args;
  merge_arguments_sequence(Args& args) : args(args) {}

  template <typename T>
  struct result;

  template <typename S, typename Iter>
  struct result<merge_arguments_sequence(S const&, Iter&)>
  {
    typedef typename boost::remove_reference<typename fusion::result_of::deref<Iter>::type>
    ::type::untagged deref_type;
    typedef typename type_tag::is_out_type_tag<deref_type>::type is_out;
    typedef typename type_tag::value_type<deref_type>::type value_type;
    typedef typename mpl::if_
    <is_out, typename boost::remove_reference<value_type>::type
     , typename boost::add_reference<value_type>::type
    >::type next_value_type;
    typedef typename fusion::result_of::push_back
    <S const, next_value_type>::type type;
  };

  template <typename S, typename Iter>
  typename result<merge_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i, mpl::false_) const
  {
    typedef result<merge_arguments_sequence(S const&, Iter&)> result_;
    typedef typename boost::remove_const<Iter>::type iterator;
    typedef typename mpl::begin<Seq const>::type begin_iterator;
    typedef mpl::iterator_range<begin_iterator, mpl::fusion_iterator<iterator> > before_range;
    typedef typename mpl::count_if<before_range, type_tag::is_not_out_type_tag<mpl::_1> >::type not_outs;
    // std::cout << "is NOT out, copying from parsed args. Type: "
    //           << typeid(typename result_::next_value_type).name()
    //           << " not_outs " << not_outs::value << std::endl;
    // return typename result_::type(fusion::at<not_outs>(args), s);
    return fusion::push_back
      <S, typename result_::next_value_type>
      (s, fusion::at<not_outs>(args));
  }  

  template <typename S, typename Iter>
  typename result<merge_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i, mpl::true_) const
  {
    typedef result<merge_arguments_sequence(S const&, Iter&)> result_;
    // std::cout << "IS out, default constructing argument. Type: "
    //           << typeid(typename result_::next_value_type).name() << std::endl;
    // return typename result_::type(typename result_::next_value_type(), s);
    return fusion::push_back(s, typename result_::next_value_type());
  }  

  template <typename S, typename Iter>
  typename result<merge_arguments_sequence(S const&, Iter&)>::type
  operator()(S const& s, Iter& i) const
  {
    typedef result<merge_arguments_sequence(S const&, Iter&)> result_;
    typedef typename result_::is_out is_out;
    return (*this)(s, i, is_out());
  }
};

template <typename R, typename SeqParam, typename T, typename F>
void handle_request_body(struct orb orb, T* self, F f, std::size_t align_offset
                         , const char*& rq_first, const char* rq_last
                         , bool little_endian, reply& r)
{
  // std::cout << "handle_request_body " << typeid(f).name() << " align_offset " << align_offset << std::endl;
  typedef typename mpl::lambda<type_tag::is_not_out_type_tag<mpl::_1> >::type
    is_not_out_lambda;
  typedef typename mpl::copy_if<SeqParam, is_not_out_lambda>::type not_out_params;

  typedef typename mpl::transform
    <not_out_params, type_tag::original_type<mpl::_1> >::type
    mpl_parse_argument_types;

  typedef typename fusion::result_of::as_vector<mpl_parse_argument_types>::type
    parse_argument_types;

  parse_argument_types parse_arguments;

  morbid::arguments_traits arguments_traits(orb);
  typedef giop::grammars::arguments<iiop::parser_domain
                                    , const char*, not_out_params
                                    , parse_argument_types>
    arguments_grammar;
  arguments_grammar arguments_grammar_(arguments_traits);

  namespace qi = boost::spirit::qi;
  if(qi::parse(rq_first, rq_last
               , giop::compile<iiop::parser_domain>
               (iiop::aligned(align_offset)[arguments_grammar_(giop::endian(little_endian))])
               , parse_arguments)
     && rq_first == rq_last)
  {
    // std::cout << "Parsed arguments correctly " << typeid(parse_arguments).name() << std::endl;
    // std::cout << "Arguments parsed " << parse_arguments << std::endl;
    // std::cout << "handle_request_body not_out_params " << typeid(parse_argument_types).name() << std::endl;

    typedef typename mpl::copy_if<SeqParam, type_tag::is_out_type_tag<mpl::_1> >::type out_params;

    typedef typename mpl::transform
      <SeqParam
       , tag<mpl::_1> >::type tagged_arguments;
    typedef typename fusion::result_of::as_vector<tagged_arguments>::type fusion_tagged_arguments;
    fusion_tagged_arguments const fusion_tagged_arguments_;
    typedef merge_arguments_sequence
      <fusion_tagged_arguments
       , out_params
       , parse_argument_types> merge_arguments;
    typedef typename boost::remove_reference
      <typename fusion::result_of::iter_fold
      <fusion_tagged_arguments const
       , fusion::vector0<>
       ,  merge_arguments>::type>::type arguments_type;
    arguments_type arguments = fusion::iter_fold(fusion_tagged_arguments_
                                                 , fusion::vector0<>()
                                                 , merge_arguments(parse_arguments));
    typedef typename fusion::result_of::as_vector<arguments_type>::type
      arguments_vector_type;
    arguments_vector_type arguments_vector = fusion::as_vector(arguments);
    // Calling function with arguments
    fusion::fused<F> fused(f);
    // Create reply
    handle_request_reply<R, SeqParam>(orb, fused, self, r, arguments_vector
                                      , mpl::identity<R>());
  }
  else
  {
    throw std::runtime_error("Error");
  }
}

}

#endif
