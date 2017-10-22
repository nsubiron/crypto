#include "crypto/password_digest.h"

#include <gtest/gtest.h>

using namespace crypto;

TEST(password_digest, basic) {
  auto pwd = secure_string::unsafe_make("a super secret password");
  const password_digest<> digest{pwd};
  EXPECT_TRUE(digest == pwd);
  auto another_pwd = secure_string::unsafe_make("a super secret password");
  password_digest<> another_digest{another_pwd};
  password_digest<> same_digest = digest;
  EXPECT_TRUE(digest == another_digest);
  EXPECT_TRUE(digest == same_digest);
  EXPECT_TRUE(digest == another_pwd);
  auto different_pwd = secure_string::unsafe_make("a different password");
  EXPECT_TRUE(digest != different_pwd);
  same_digest = password_digest<>(different_pwd);
  EXPECT_TRUE(digest != same_digest) << '\n'
      << digest.to_hex_string() << '\n'
      << same_digest.to_hex_string() << '\n';
}
