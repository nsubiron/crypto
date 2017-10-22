// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "buffer_view.h"

#include <array>
#include <string>

namespace crypto {

  using sha256_digest = std::array<byte, 32u>;

  using sha512_digest = std::array<byte, 64u>;

  void digest(const_buffer_view buffer, sha256_digest &digest);

  void digest(const_buffer_view buffer, sha512_digest &digest);

  void zeroize(mutable_buffer_view buffer);

} // namespace crypto
