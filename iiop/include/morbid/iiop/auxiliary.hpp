/* (c) Copyright 2012,2013 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_AUXILIARY_HPP
#define MORBID_IIOP_AUXILIARY_HPP

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail {

template <typename F, typename T, std::size_t N, typename Sequence>
struct pass_container<F, boost::array<T, N>, Sequence>
{
  typedef typename F::context_type context_type;
  typedef typename F::iterator_type iterator_type;
  typedef boost::array<T, N> attr_type;

  pass_container(F const& f_, attr_type& attr_)
    : index(new std::size_t(0u)), f(f_), attr(attr_) {}

  // this is for the case when the current element exposes an attribute
  // which is pushed back onto the container
  template <typename Component>
  bool dispatch_container(Component const& component, mpl::false_) const
  {
    std::cout << "pass_container::dispatch_container index " << (*index) << std::endl;
    // synthesized attribute needs to be default constructed
    typename traits::container_value<attr_type>::type val =
      typename traits::container_value<attr_type>::type();

    iterator_type save = f.first;
    bool r = f(component, val);
    if (!r)
    {
      std::cout << "!r" << std::endl;
      // push the parsed value into our attribute
      attr[(*index)++] = val;
      f.first = save;
    }
    else
      std::cout << "r" << std::endl;
    return r;
  }

  // this is for the case when the current element is able to handle an 
  // attribute which is a container itself, this element will push its 
  // data directly into the attribute container
  template <typename Component>
  bool dispatch_container(Component const& component, mpl::true_) const
  {
    std::cout << "pass_container::dispatch_container false index " << (*index) << std::endl;
    return f(component, attr);
  }

  ///////////////////////////////////////////////////////////////////////
  // this is for the case when the current element doesn't expect an 
  // attribute
  template <typename Component>
  bool dispatch_attribute(Component const& component, mpl::false_) const
  {
    std::cout << "pass_container::dispatch_attribute false index " << (*index) << std::endl;
    return f(component, unused);
  }

  // the current element expects an attribute
  template <typename Component>
  bool dispatch_attribute(Component const& component, mpl::true_) const
  {
    std::cout << "pass_container::dispatch_attribute true index " << (*index) << std::endl;
    typedef typename traits::container_value<attr_type>::type value_type;
    typedef typename traits::attribute_of<
      Component, context_type, iterator_type>::type
      rhs_attribute;

    // this predicate detects, whether the attribute of the current 
    // element is a substitute for the value type of the container
    // attribute 
    typedef mpl::and_<
      traits::handles_container<
        Component, attr_type, context_type, iterator_type> 
      , traits::pass_through_container<
        attr_type, value_type, rhs_attribute, Sequence, qi::domain>
      > predicate;

    return dispatch_container(component, predicate());
  }

  // Dispatches to dispatch_main depending on the attribute type
  // of the Component
  template <typename Component>
  bool operator()(Component const& component) const
  {
    std::cout << "pass_container::operator() index " << (*index) << std::endl;
    // we need to dispatch depending on the type of the attribute
    // of the current element (component). If this is has no attribute
    // we shouldn't pass an attribute at all.
    typedef typename traits::not_is_unused<
      typename traits::attribute_of<
        Component, context_type, iterator_type
        >::type
      >::type predicate;

    // ensure the attribute is actually a container type
    traits::make_container(attr);

    return dispatch_attribute(component, predicate());
  }

  boost::shared_ptr<std::size_t> index;
  F f;
  attr_type& attr;
private:
  // silence MSVC warning C4512: assignment operator could not be generated
  pass_container& operator= (pass_container const&);
};

} } } }

namespace morbid { namespace iiop {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace karma = spirit::karma;

namespace parser {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef qi::eps_parser result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};

template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef qi::semantic_predicate result_type;
  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args) ? true : false);
  }
};

template <typename Terminal, typename Actor, int Arity, typename Modifiers>
struct make_primitive<spirit::lazy_terminal<Terminal, Actor, Arity>, Modifiers>
{
  typedef qi::lazy_parser<Actor, Modifiers> result_type;
  result_type operator()(spirit::lazy_terminal<Terminal, Actor, Arity> const& lt
                         , Modifiers const& modifiers) const
  {
    return result_type(lt.actor, modifiers);
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::tag::repeat, Subject, Modifiers>
{
  typedef qi::kleene<Subject> result_type;
  result_type operator()(spirit::unused_type, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector1<T> >, Subject, Modifiers>
{
  typedef qi::exact_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector2<T, T> >, Subject, Modifiers>
{
  typedef qi::finite_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;
  
  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject,
                       iterator_type(
                                     fusion::at_c<0>(term.args)
                                     , fusion::at_c<1>(term.args)
                                     )
                       );
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<spirit::terminal_ex<spirit::tag::repeat, fusion::vector2<T, spirit::inf_type> >, Subject, Modifiers>
{
  typedef qi::infinite_iterator<T> iterator_type;
  typedef qi::repeat_parser<Subject, iterator_type> result_type;
  
  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

///////////////////////////////////////////////////////////////////////////
// Parser generator: make_xxx function (objects)
///////////////////////////////////////////////////////////////////////////
template <typename Expr, typename Exposed, typename Transformed
          , typename Modifiers>
struct make_primitive<
  spirit::tag::stateful_tag<Expr, spirit::tag::attr_cast, Exposed, Transformed>, Modifiers>
{
  typedef typename spirit::result_of::compile<parser_domain, Expr>::type
    expr_type;
  typedef qi::attr_cast_parser<Exposed, Transformed, expr_type> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    typedef spirit::tag::stateful_tag<
      expr_type, spirit::tag::attr_cast, Exposed, Transformed> tag_type;
    using spirit::detail::get_stateful_data;
    return result_type(/*giop::compile<parser_domain>*/
                       /*(*/get_stateful_data<tag_type>::call(term))/*)*/;
  }
};

}

namespace generator {

template <typename Modifiers>
struct make_primitive<spirit::tag::eps, Modifiers>
{
  typedef karma::eps_generator result_type;
  result_type operator()(spirit::unused_type, spirit::unused_type) const
  {
    return result_type();
  }
};
  
template <typename Modifiers, typename A0>
struct make_primitive<
  spirit::terminal_ex<spirit::tag::eps, fusion::vector1<A0> >
  , Modifiers>
{
  typedef karma::semantic_predicate result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, spirit::unused_type) const
  {
    return result_type(fusion::at_c<0>(term.args));
  }
};

template <typename Subject, typename Modifiers>
struct make_directive<spirit::tag::repeat, Subject, Modifiers>
{
  typedef typename mpl::if_<
    karma::detail::get_stricttag<Modifiers>
    , karma::strict_kleene<Subject>, karma::kleene<Subject>
  >::type result_type;

  result_type operator()(spirit::unused_type, Subject const& subject
                         , spirit::unused_type) const
  {
    return result_type(subject);
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<
  spirit::terminal_ex<spirit::tag::repeat, fusion::vector1<T> >, Subject, Modifiers>
{
  typedef karma::exact_iterator<T> iterator_type;

  typedef typename mpl::if_<
    karma::detail::get_stricttag<Modifiers>
    , karma::strict_repeat_generator<Subject, iterator_type>
    , karma::repeat_generator<Subject, iterator_type>
  >::type result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<
  spirit::terminal_ex<spirit::tag::repeat, fusion::vector2<T, T> >, Subject, Modifiers>
{
  typedef karma::finite_iterator<T> iterator_type;

  typedef typename mpl::if_<
    karma::detail::get_stricttag<Modifiers>
    , karma::strict_repeat_generator<Subject, iterator_type>
    , karma::repeat_generator<Subject, iterator_type>
  >::type result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject,
                       iterator_type(fusion::at_c<0>(term.args)
                                     , fusion::at_c<1>(term.args)
                                    )
                      );
  }
};

template <typename T, typename Subject, typename Modifiers>
struct make_directive<
  spirit::terminal_ex<spirit::tag::repeat
                      , fusion::vector2<T, spirit::inf_type> >, Subject, Modifiers>
{
  typedef karma::infinite_iterator<T> iterator_type;

  typedef typename mpl::if_<
    karma::detail::get_stricttag<Modifiers>
    , karma::strict_repeat_generator<Subject, iterator_type>
    , karma::repeat_generator<Subject, iterator_type>
  >::type result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, Subject const& subject, spirit::unused_type) const
  {
    return result_type(subject, fusion::at_c<0>(term.args));
  }
};

}

} }

namespace boost { namespace spirit {

template <>
struct use_terminal< ::morbid::iiop::parser_domain, spirit::tag::eps>       // enables eps
  : mpl::true_ {};

template <typename A0>
struct use_terminal
  < ::morbid::iiop::parser_domain
    , terminal_ex<tag::eps, fusion::vector1<A0> > // enables eps(bool-condition)
  > : is_convertible<A0, bool> {};

template <>                                     // enables eps(f)
struct use_lazy_terminal<
  ::morbid::iiop::parser_domain, tag::eps, 1 /*arity*/
  > : mpl::true_ {};

// enables eps
template <>
struct use_terminal< ::morbid::iiop::generator_domain, tag::eps>
  : mpl::true_ {};

// enables eps(bool-condition)
template <typename A0>
struct use_terminal
 < ::morbid::iiop::generator_domain
   , terminal_ex<tag::eps, fusion::vector1<A0> > > 
: is_convertible<A0, bool> {};

// enables lazy eps(f)
template <>
struct use_lazy_terminal< ::morbid::iiop::generator_domain, tag::eps, 1>
  : mpl::true_ {};

template <>
struct use_directive< ::morbid::iiop::generator_domain, tag::repeat>   // enables repeat[p]
  : mpl::true_ {};

    template <typename T>
    struct use_directive< ::morbid::iiop::generator_domain
      , terminal_ex<tag::repeat                     // enables repeat(exact)[p]
        , fusion::vector1<T> >
    > : mpl::true_ {};

    template <typename T>
    struct use_directive< ::morbid::iiop::generator_domain
      , terminal_ex<tag::repeat                     // enables repeat(min, max)[p]
        , fusion::vector2<T, T> >
    > : mpl::true_ {};

    template <typename T>
    struct use_directive< ::morbid::iiop::generator_domain
      , terminal_ex<tag::repeat                     // enables repeat(min, inf)[p]
        , fusion::vector2<T, inf_type> >
    > : mpl::true_ {};

    template <>                                     // enables *lazy* repeat(exact)[p]
    struct use_lazy_directive<
         ::morbid::iiop::generator_domain
      , tag::repeat
      , 1 // arity
    > : mpl::true_ {};

    template <>                                     // enables *lazy* repeat(min, max)[p]
    struct use_lazy_directive<                      // and repeat(min, inf)[p]
         ::morbid::iiop::generator_domain
      , tag::repeat
      , 2 // arity
    > : mpl::true_ {};

template <>
struct use_directive< ::morbid::iiop::parser_domain, tag::repeat>   // enables repeat[p]
  : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector1<T> >    // enables repeat(exact)[p]
 > : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector2<T, T> > // enables repeat(min, max)[p]
 > : mpl::true_ {};

template <typename T>
struct use_directive
 < ::morbid::iiop::parser_domain
   , terminal_ex<tag::repeat, fusion::vector2<T, inf_type> > // enables repeat(min, inf)[p]
 > : mpl::true_ {};

template <>                                     // enables *lazy* repeat(exact)[p]
struct use_lazy_directive
 < ::morbid::iiop::parser_domain
   , tag::repeat
   , 1 // arity
 > : mpl::true_ {};

template <>                                     // enables *lazy* repeat(min, max)[p]
struct use_lazy_directive                       // and repeat(min, inf)[p]
 < ::morbid::iiop::parser_domain
   , tag::repeat
   , 2 // arity
 > : mpl::true_ {};

// enables attr_cast<>() pseudo parser
template <typename Expr, typename Exposed, typename Transformed>
struct use_terminal< ::morbid::iiop::parser_domain
                    , tag::stateful_tag<Expr, tag::attr_cast, Exposed, Transformed> >
 : mpl::true_ {};

} }

#endif
