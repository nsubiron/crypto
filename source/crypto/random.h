// Copyright (c) 2017 N Subiron Montoro. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "random_engine_adaptor.h"
#include "randutils.h"

namespace crypto {

  using default_random_engine = random_engine_adaptor<randutils::default_rng>;

  using mt19937 = random_engine_adaptor<randutils::mt19937_rng>;

} // namespace crypto
