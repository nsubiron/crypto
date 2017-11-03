crypto
======

Few handy wrappers around [OpenSSL](https://www.openssl.org/)'s libcrypto.

Includes

  * Simple secure string
  * Password digest class
  * Random number generator adaptor

Build
-----

Build with cmake. For default cmake configuration use the provided Makefile.
Requires C++14 or later.

Unit tests
----------

The unit tests require [Google Test](https://github.com/google/googletest), the
setup script downloads and compiles GTest to use with the provided Makefile

    $ ./Setup.sh
    $ make check

Usage
-----

All the include files are installed under `$installdir/includes`. Static
libraries are installed under `$installdir/includes`. Link against
`wcrypto` (`wcryptod` for debug) and OpenSSL's crypto. E.g.

    $ clang++ -I$installdir/include -L$installdir/lib ... -lwcrypto -lcrypto

#### Secure string

A super basic secure string. Its contents are zeroized on destruction. It is
implemented from scratch to avoid small string optimizations, therefore most of
the functionality of std::string is lost. It just provides basic functionality
to move the string around and append two strings.

It cannot be constructed directly, but through its static methods
`clean_buffer_and_make` and `unsafe_make`. This is meant to avoid unsafe usage
of the string go unnoticed.

```cpp
#include "crypto/secure_string.h"

#include <unistd.h>

int main() {
  using namespace crypto;

  char *buffer = getpass("Password: ");
  auto str0 = secure_string::clean_buffer_and_make(buffer);

  auto str1 = secure_string::unsafe_make(" with some not so secret text");

  auto str = str0 + str1;
}
```

#### Password digest

A password digest object to store digested passwords. Allows comparison with
other password digests and clear passwords.

This object can be copied around without copying the holding data. A single
const copy of the data is shared among all the copies of the password_digest
using shared_ptr.

```cpp
#include "crypto/password_digest.h"

#include <iostream>
#include <unistd.h>

int main() {
  using namespace crypto;

  char *unsafe_buffer = getpass("Password: ");
  auto password = secure_string::clean_buffer_and_make(unsafe_buffer);

  auto digest = password_digest<sha256_digest>(password);

  unsafe_buffer = getpass("Confirm password: ");
  auto confirm_password = secure_string::clean_buffer_and_make(unsafe_buffer);

  if (digest != confirm_password)
    std::cerr << "Passwords do not match!" << std::endl;
}
```

#### Random engine adaptor

The `random_engine_adaptor` implements most of the standard random utilities as
member functions of the random engine. Two default random engines are provided
(`crypto::default_random_engine` and `crypto::mt19937`) based on
[_randutils.hpp_](https://gist.github.com/imneme/540829265469e673d045). When
default constructed, these two are seeded with a random seed.

```cpp
#include "crypto/random.h"

#include <iostream>
#include <vector>

int main() {
  crypto::mt19937 rng0;

  std::cout << rng0.uniform<double>()     << '\n'
            << rng0.uniform<int>(-10, 10) << '\n'
            << rng0.bernoulli(0.6)        << '\n';

  std::seed_seq seed{1, 2, 3, 4, 5, 6, 7, 8, 9};
  crypto::default_random_engine rng1(seed);

  std::vector<int> v{1, 2, 3, 4, 5};
  rng1.shuffle(v);

  // Using directly with std random engines.
  crypto::random_engine_adaptor<std::minstd_rand> rng2;

  std::cout << rng2.choice(v) << '\n';
}
```

License
-------

_crypto_ is distributed under MIT License.

[_randutils.h_](https://gist.github.com/imneme/540829265469e673d045) Copyright
(c) 2015 Melissa E. O'Neill, distributed under MIT License.
