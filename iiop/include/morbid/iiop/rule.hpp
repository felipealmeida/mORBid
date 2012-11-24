/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MORBID_IIOP_RULE_HPP
#define MORBID_IIOP_RULE_HPP

#include <morbid/giop/rule.hpp>

#include <boost/spirit/home/karma.hpp>
#include <boost/spirit/home/support.hpp>

namespace morbid { namespace iiop {

namespace fusion = boost::fusion;

namespace generator {

template <typename R, typename Params>
struct rule_generator
{
  rule_generator(R const& rule, Params const& params)
    : rule(&rule), params(params)
  {
    std::cout << "rule_generator::rule_generator rule " << &rule
              << " this " << this
              << " this rule " << this->rule << std::endl;
  }

  typedef typename R::rule_type::properties properties;

  template <typename Context, typename Iterator>
  struct attributes
  {
    typedef spirit::unused_type type;
  };

  template <typename Context, typename Delimiter, typename Attr>
  bool generate(typename R::rule_type::output_iterator& sink, Context& caller_context
                , Delimiter const& d, Attr const& attr) const
  {
    typedef typename R::rule_type::attr_type attr_type;
    BOOST_MPL_ASSERT((boost::is_same<Attr, attr_type>));
    // Create an attribute if none is supplied.
    typedef spirit::traits::make_attribute<attr_type, Attr>
      make_attribute;
    typedef spirit::traits::transform_attribute<
      typename make_attribute::type, attr_type, karma::domain>
      transform;

    typename transform::type attr_ =
      spirit::traits::pre_transform<karma::domain, attr_type>
      (make_attribute::call(attr));

    typedef typename R::rule_type::context_type context_type;
    // If you are seeing a compilation error here, you are probably
    // trying to use a rule or a grammar which has inherited
    // attributes, without passing values for them.
    context_type context(attr_, params, caller_context);

    std::cout << "generate for rule " << rule << std::endl;
    std::cout << "generate this " << this << std::endl;

    // If you are seeing a compilation error here stating that the
    // third parameter can't be converted to a karma::reference
    // then you are probably trying to use a rule or a grammar with
    // an incompatible delimiter type.
    if (rule->f(sink, context, d))
    {
      typedef typename R::rule_type::delimiter_type delimiter_type;
      // do a post-delimit if this is an implied verbatim
      if (boost::is_same<delimiter_type, spirit::unused_type>::value)
        karma::delimit_out(sink, d);

      return true;
    }
    return false;
  }  

  R const* rule;
  Params params;
};

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Modifiers, typename Enable>
struct make_primitive<boost::reference_wrapper
                      <giop::rule< generator_domain, Iterator, T1, T2, T3, T4>const>
                       , Modifiers, Enable>
{
  typedef giop::rule< generator_domain, Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_generator<rule_type, fusion::vector0<> > result_type;

  result_type operator()(rule_type const& val, boost::spirit::unused_type) const
  {
    return result_type(val);
  }
};

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Modifiers, typename V, typename Enable>
struct make_primitive<karma::parameterized_nonterminal
                      <
                        giop::rule< generator_domain, Iterator, T1, T2, T3, T4>
                        , V
                      >
                      , Modifiers, Enable>
{
  typedef giop::rule< generator_domain, Iterator, T1, T2, T3, T4> rule_type;
  typedef rule_generator<rule_type, V> result_type;

  template <typename Terminal>
  result_type operator()(Terminal const& term, boost::spirit::unused_type) const
  {
    std::cout << "rule this: " << &term.ref.get() << std::endl;
    return result_type(term.ref.get(), term.params);
  }
};

} } }

namespace boost { namespace spirit {

template <typename Iterator, typename T1, typename T2, typename T3, typename T4, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , boost::reference_wrapper
                     < ::morbid::giop::rule
                       < ::morbid::iiop::generator_domain, Iterator, T1, T2, T3, T4>const>
                     , Enable> : mpl::true_ {};
template <typename Iterator, typename T1, typename T2, typename T3, typename T4
          , typename V, typename Enable>
struct use_terminal< ::morbid::iiop::generator_domain
                     , karma::parameterized_nonterminal
                     < ::morbid::giop::rule
                       < ::morbid::iiop::generator_domain, Iterator, T1, T2, T3, T4>
                       , V>
                     , Enable> : mpl::true_ {};

template <typename Iterator, typename T1, typename T2, typename T3, typename T4>
struct use_lazy_directive< ::morbid::iiop::generator_domain
                           , boost::reference_wrapper
                           < ::morbid::giop::rule
                             < ::morbid::iiop::generator_domain, Iterator, T1, T2, T3, T4>const>
                           , 1>
  : mpl::true_ {};

} }

#endif
