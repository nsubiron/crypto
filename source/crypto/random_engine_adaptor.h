// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <type_traits>

namespace crypto {

namespace detail {

  template <typename T, bool IS_FLOATING_POINT = std::is_floating_point<T>::value>
  struct uniform_distribution_impl;

  template <typename T>
  struct uniform_distribution_impl<T, true> {
    static constexpr T min = 0.0;
    static constexpr T max = 1.0;
    using distribution = std::uniform_real_distribution<T>;
  };

  template <typename T>
  struct uniform_distribution_impl<T, false> {
    static constexpr T min = 0.0;
    static constexpr T max = std::numeric_limits<T>::max();
    using distribution = std::uniform_int_distribution<T>;
  };

  template <typename T>
  struct uniform_distribution : public uniform_distribution_impl<T> {};

} // detail


  /// Implements most of the standard random utilities as member functions of
  /// the random engine.
  template <typename RANDOM_ENGINE>
  class random_engine_adaptor : private RANDOM_ENGINE {
  public:

    using engine_type = RANDOM_ENGINE;

    template <typename ... ARGS>
    explicit random_engine_adaptor(ARGS&&...args) : engine_type(std::forward<ARGS>(args)...) {}

    random_engine_adaptor(const random_engine_adaptor &) = default;

    using engine_type::operator();
    using engine_type::seed;
    using engine_type::min;
    using engine_type::max;
    using typename engine_type::result_type;

    engine_type &engine() {
      return *this;
    }

    /// @name Uniform distributions
    /// @{

    /// Produces random values uniformly distributed on the closed interval
    /// [min, max].
    ///
    /// Default arguments are [0.0, 1.0] for floating points types, and
    /// [0, std::numeric_limits<T>::max()] for integral types.
    ///
    /// The effect is undefined if T is not one of short, int, long, long
    /// long, unsigned short, unsigned int, unsigned long, unsigned long long,
    /// float, double, or long double.
    template <typename T>
    T uniform(
        T min = detail::uniform_distribution<T>::min,
        T max = detail::uniform_distribution<T>::max) {
      using dist = typename detail::uniform_distribution<T>::distribution;
      return dist(min, max)(*this);
    }

    bool uniform_bool() {
      return (uniform<int>(0, 1) == 1);
    }

    bool uniform_bool(double weight) {
      return (uniform<double>() < weight);
    }

    /// @}
    /// @name Bernoulli distributions
    /// @{

    bool bernoulli(double p = 0.5) {
      return std::bernoulli_distribution(p)(*this);
    }

    template <typename IntType = int>
    IntType binomial(IntType t = 1, double p = 0.5) {
      return std::binomial_distribution<IntType>(t, p)(*this);
    }

    template <typename IntType = int>
    IntType negative_binomial(IntType k = 1, double p = 0.5) {
      return std::negative_binomial_distribution<IntType>(k, p)(*this);
    }

    template <typename IntType = int>
    IntType geometric(double p = 0.5) {
      return std::geometric_distribution<IntType>(p)(*this);
    }

    /// @}
    /// @name Poisson distributions
    /// @{

    template <typename IntType = int>
    IntType poisson(double mean = 1.0) {
      return std::poisson_distribution<IntType>(mean)(*this);
    }

    template <typename RealType = double>
    RealType exponential(RealType lambda = 1.0) {
      return std::exponential_distribution<RealType>(lambda)(*this);
    }

    template <typename RealType = double>
    RealType gamma(RealType alpha = 1.0, RealType beta = 1.0) {
      return std::gamma_distribution<RealType>(alpha, beta)(*this);
    }

    template <typename RealType = double>
    RealType weibull(RealType a = 1.0, RealType b = 1.0) {
      return std::weibull_distribution<RealType>(a, b)(*this);
    }

    template <typename RealType = double>
    RealType extreme_value(RealType a = 0.0, RealType b = 1.0) {
      return std::extreme_value_distribution<RealType>(a, b)(*this);
    }

    /// @}
    /// @name Normal distributions
    /// @{

    template <typename RealType = double>
    RealType normal(RealType mean = 0.0, RealType stddev = 1.0) {
      return std::normal_distribution<RealType>(mean, stddev)(*this);
    }

    template <typename RealType = double>
    RealType lognormal(RealType m = 0.0, RealType s = 1.0) {
      return std::lognormal_distribution<RealType>(m, s)(*this);
    }

    template <typename RealType = double>
    RealType chi_squared(RealType n = 1.0) {
      return std::chi_squared_distribution<RealType>(n)(*this);
    }

    template <typename RealType = double>
    RealType cauchy(RealType a = 0.0, RealType b = 1.0) {
      return std::cauchy_distribution<RealType>(a, b)(*this);
    }

    template <typename RealType = double>
    RealType fisher_f(RealType m = 1.0, RealType n = 1.0) {
      return std::fisher_f_distribution<RealType>(m, n)(*this);
    }

    template <typename RealType = double>
    RealType student_t(RealType n = 1.0) {
      return std::student_t_distribution<RealType>(n)(*this);
    }

    /// @}
    /// @name Sampling distributions
    /// @{

    template <typename IntType = int, typename ... ARGS>
    IntType discrete(ARGS&&...args) {
      return std::discrete_distribution<IntType>(std::forward<ARGS>(args)...)(*this);
    }

    template <typename RealType = double, typename ... ARGS>
    RealType piecewise_constant(ARGS&&...args) {
      return std::piecewise_constant_distribution<RealType>(std::forward<ARGS>(args)...)(*this);
    }

    template <typename RealType = double, typename ... ARGS>
    RealType piecewise_linear(ARGS&&...args) {
      return std::piecewise_linear_distribution<RealType>(std::forward<ARGS>(args)...)(*this);
    }

    /// @}
    /// @name Container utils
    /// @{

    template <typename InputIt>
    InputIt choice(InputIt begin, InputIt end) {
      auto distance = std::distance(begin, end);
      if (distance < 2)
        return begin;
      auto position = uniform<decltype(distance)>(0, --distance);
      std::advance(begin, position);
      return begin;
    }

    template <typename Range>
    auto choice(Range &&range) {
      return *choice(std::begin(range), std::end(range));
    }

    template <typename RandomIt>
    void shuffle(RandomIt begin, RandomIt end) {
      std::shuffle(begin, end, *this);
    }

    template <typename Range>
    void shuffle(Range &&range) {
      shuffle(std::begin(range), std::end(range));
    }

    /// @}
  };

} // namespace crypto
