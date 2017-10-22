#include "crypto/buffer_view.h"

#include <gtest/gtest.h>

#include <array>
#include <cstring>
#include <string>

using namespace crypto;

static bool copy_buffer(const_buffer_view src, mutable_buffer_view dst) {
  if (dst.size() >= src.size()) {
    std::memcpy(dst.data(), src.data(), src.size());
    return true;
  }
  return false;
}

static bool compare_buffers(const_buffer_view lhs, const_buffer_view rhs) {
  if (lhs.size() != rhs.size())
    return false;
  for (auto i = 0u; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i])
      return false;
  }
  return true;
}

TEST(buffer_view, basic) {
  const std::string str = "Hello world!";
  std::array<char, 20u> array;
  EXPECT_TRUE(copy_buffer(str, array));
  EXPECT_FALSE(compare_buffers(str, array));
  auto buffer = buffer_view::make_mutable(array.data(), str.size());
  EXPECT_TRUE(compare_buffers(str, buffer));
  EXPECT_TRUE(copy_buffer("Hola ", buffer));
  EXPECT_TRUE(compare_buffers("Hola  world!", buffer));
}

TEST(buffer_view, vector) {
  constexpr auto size = 10000u;
  const auto c_vector = []() {
    std::vector<float> vector;
    vector.reserve(size);
    for (auto i = 0u; i < size; ++i) {
      vector.push_back(float(i));
    }
    return vector;
  }();
  std::vector<float> m_vector(size);
  EXPECT_FALSE(compare_buffers(c_vector, m_vector));
  EXPECT_TRUE(copy_buffer(c_vector, m_vector));
  EXPECT_TRUE(compare_buffers(c_vector, m_vector));
}

