#! /bin/bash
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

export C_COMPILER=clang
export COMPILER=clang++

mkdir -p build
pushd build >/dev/null

# ==============================================================================
# -- Get and compile GTest -----------------------------------------------------
# ==============================================================================

# Get googletest source
if [[ ! -d "googletest-source" ]]; then
  echo "Retrieving googletest..."
  git clone --depth=1 -b release-1.8.0 https://github.com/google/googletest.git googletest-source
else
  echo "Folder googletest-source already exists, skipping git clone..."
fi

pushd googletest-source >/dev/null

cmake -H. -B./build \
    -DCMAKE_C_COMPILER=${C_COMPILER} -DCMAKE_CXX_COMPILER=${COMPILER} \
    -DCMAKE_INSTALL_PREFIX="../googletest-install" \
    -G "Ninja"

pushd build >/dev/null
ninja
ninja install
popd >/dev/null

popd >/dev/null

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

popd >/dev/null
popd >/dev/null

set +x
echo ""
echo "Success!"
