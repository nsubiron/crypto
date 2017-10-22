INSTALL_FOLDER=$(CURDIR)/install
BASE_BUILD_FOLDER=$(CURDIR)/build/crypto
GTEST_INSTALL_PATH=$(CURDIR)/build/googletest-install
MY_CMAKE_FOLDER=$(CURDIR)
MY_CMAKE_FLAGS=\
    -B"$(BUILD_FOLDER)" \
    -DCMAKE_INSTALL_PREFIX="$(INSTALL_FOLDER)" \
    -DGTEST_INSTALL_PATH="$(GTEST_INSTALL_PATH)"

ifeq ($(OS),Windows_NT)
BUILD_RULE=build_windows
else
BUILD_RULE=build_linux
EXTRA_CMAKE_FLAGS=-DCMAKE_CXX_COMPILER="clang++" -DCMAKE_CXX_FLAGS="-std=c++1z -Wall -Wextra"
endif

default: release

### Build ######################################################################

debug: BUILD_FOLDER=$(BASE_BUILD_FOLDER)/debug
debug: MY_CMAKE_FLAGS+=-DCMAKE_BUILD_TYPE=Debug
debug: $(BUILD_RULE)

release: BUILD_FOLDER=$(BASE_BUILD_FOLDER)/release
release: MY_CMAKE_FLAGS+=-DCMAKE_BUILD_TYPE=Release
release: $(BUILD_RULE)

build_linux: MY_CMAKE_FLAGS+=-G "Ninja"
build_linux: call_cmake
	@cd $(BUILD_FOLDER) && ninja && ninja install

build_windows: MY_CMAKE_FLAGS+=-G "NMake Makefiles"
build_windows: call_cmake
	@cd $(BUILD_FOLDER) && nmake && nmake install

call_cmake:
	@mkdir -p $(BUILD_FOLDER)
	@cd $(BUILD_FOLDER) && cmake $(MY_CMAKE_FLAGS) $(EXTRA_CMAKE_FLAGS) "$(MY_CMAKE_FOLDER)"

### Docs #######################################################################

docs:
	@doxygen
	@echo "Documentation index at ./build/doxygen/html/index.html"

### Clean ######################################################################

clean:
	@rm -Rf $(BASE_BUILD_FOLDER) $(INSTALL_FOLDER) ./build/doxygen

### Test #######################################################################

check: check_release

check_release: release
	@$(BASE_BUILD_FOLDER)/release/crypto_test_release

check_debug: debug
	@$(BASE_BUILD_FOLDER)/debug/crypto_test_debug
