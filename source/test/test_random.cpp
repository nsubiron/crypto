#include "crypto/random.h"

#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <vector>

template <typename T, typename GEN>
static std::vector<T> generate_vector(size_t size, GEN &&generator) {
  std::vector<T> v;
  v.reserve(size);
  std::generate_n(std::back_inserter(v), size, std::forward<GEN>(generator));
  return v;
}

// This mostly tests that everything compiles.
template <typename T>
static bool test_engine() {
  using namespace crypto;

  {
    random_engine_adaptor<T> rng;
    auto result = rng.template uniform<size_t>(0u, 100u);
    if (result > 100u)
      return false;
  }

  std::seed_seq seed{1,2,3,4,5,6,7,8,9};
  random_engine_adaptor<T> rng0(seed);
  random_engine_adaptor<T> rng1(seed);
  for (auto i = 0u; i < 10000u; ++i) {
    if (rng0.template uniform<float>() != rng1.template uniform<float>()) return false;
    if (rng0.template uniform<int>() != rng1.template uniform<int>()) return false;
    if (rng0.template uniform<size_t>() != rng1.template uniform<size_t>()) return false;
    if (rng0.template uniform_bool() != rng1.template uniform_bool()) return false;
    if (rng0.template uniform_bool(0.3) != rng1.template uniform_bool(0.3)) return false;
    if (rng0.template bernoulli() != rng1.template bernoulli()) return false;
    if (rng0.template binomial() != rng1.template binomial()) return false;
    if (rng0.template negative_binomial() != rng1.template negative_binomial()) return false;
    if (rng0.template geometric() != rng1.template geometric()) return false;
    if (rng0.template poisson() != rng1.template poisson()) return false;
    if (rng0.template exponential() != rng1.template exponential()) return false;
    if (rng0.template gamma() != rng1.template gamma()) return false;
    if (rng0.template weibull() != rng1.template weibull()) return false;
    if (rng0.template extreme_value() != rng1.template extreme_value()) return false;
    if (rng0.template normal() != rng1.template normal()) return false;
    if (rng0.template lognormal() != rng1.template lognormal()) return false;
    if (rng0.template chi_squared() != rng1.template chi_squared()) return false;
    if (rng0.template cauchy() != rng1.template cauchy()) return false;
    if (rng0.template fisher_f() != rng1.template fisher_f()) return false;
    if (rng0.template student_t() != rng1.template student_t()) return false;
  }

  constexpr auto vector_size = 200u;
  auto v0 = generate_vector<double>(vector_size, [&](){ return rng0.template uniform<double>(-100.0, 100.0); });
  auto w0 = generate_vector<double>(vector_size, [&](){ return rng0.template uniform<double>(0.0, 100.0); });
  auto v1 = generate_vector<double>(vector_size, [&](){ return rng1.template uniform<double>(-100.0, 100.0); });
  auto w1 = generate_vector<double>(vector_size, [&](){ return rng1.template uniform<double>(0.0, 100.0); });
  if (v0 != v1)
    return false;
  rng0.shuffle(v0);
  rng1.shuffle(v1);
  if (v0 != v1)
    return false;
  if (rng0.choice(v0) != rng1.choice(v1))
    return false;

  for (auto i = 0u; i < 10000u; ++i) {
    if (rng0.template discrete(v0.begin(), v0.end()) !=
        rng1.template discrete(v1.begin(), v1.end()))
      return false;
    if (rng0.template piecewise_constant(v0.begin(), v0.end(), w0.begin()) !=
        rng1.template piecewise_constant(v1.begin(), v1.end(), w1.begin()))
      return false;
    if (rng0.template piecewise_linear(v0.begin(), v0.end(), w0.begin()) !=
        rng1.template piecewise_linear(v1.begin(), v1.end(), w1.begin()))
      return false;
  }

  return true;
}

TEST(random, default_random_engine)     { ASSERT_TRUE(test_engine<crypto::default_random_engine>()); }
TEST(random, mt19937)                   { ASSERT_TRUE(test_engine<crypto::mt19937>()); }
TEST(random, std_default_random_engine) { ASSERT_TRUE(test_engine<std::default_random_engine>()); }
TEST(random, std_minstd_rand0)          { ASSERT_TRUE(test_engine<std::minstd_rand0>()); }
TEST(random, std_minstd_rand)           { ASSERT_TRUE(test_engine<std::minstd_rand>()); }
TEST(random, std_mt19937)               { ASSERT_TRUE(test_engine<std::mt19937>()); }
TEST(random, std_mt19937_64)            { ASSERT_TRUE(test_engine<std::mt19937_64>()); }
TEST(random, std_ranlux24_base)         { ASSERT_TRUE(test_engine<std::ranlux24_base>()); }
TEST(random, std_ranlux48_base)         { ASSERT_TRUE(test_engine<std::ranlux48_base>()); }
TEST(random, std_ranlux24)              { ASSERT_TRUE(test_engine<std::ranlux24>()); }
TEST(random, std_ranlux48)              { ASSERT_TRUE(test_engine<std::ranlux48>()); }
TEST(random, std_knuth_b)               { ASSERT_TRUE(test_engine<std::knuth_b>()); }
