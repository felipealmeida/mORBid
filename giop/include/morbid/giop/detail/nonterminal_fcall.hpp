/* (c) Copyright 2012 Felipe Magno de Almeida
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING
#error This file should not be included directly
#endif

#define N BOOST_PP_ITERATION()

  template <BOOST_PP_ENUM_PARAMS(N, typename A)>
  typename boost::proto::terminal<
    parameterized_nonterminal
    <domain_type
     , parameterized_subject_type
     , BOOST_PP_CAT(boost::fusion::vector, N)<BOOST_PP_ENUM_PARAMS(N, A)>
    >
  >::type
  operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& f)) const
  {
    BOOST_MPL_ASSERT_RELATION(params_size, ==, N);

    typedef BOOST_PP_CAT(boost::fusion::vector, N)<BOOST_PP_ENUM_PARAMS(N, A)> vector_type;
    typedef parameterized_nonterminal
      <domain_type, parameterized_subject_type, vector_type> parameterized_type;
    typedef typename boost::proto::terminal<parameterized_type>::type result_type;
    
    return result_type::make
      (parameterized_type
       (this->get_parameterized_subject()
        , vector_type(BOOST_PP_ENUM_PARAMS(N, f)))
       );
  }

#undef N

