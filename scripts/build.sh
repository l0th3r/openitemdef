#!/bin/bash
set -e

BUILD_TYPE=Debug
BUILD_DIR=.build
CMAKE_FLAGS=()
OPT_FLAGS=()

# Doc
DOC_REQUESTED=false

# Test and coverage
TEST_REQUESTED=false
COVR_REQUESTED=false

for arg in "$@";
  do
    if [ "$arg" == "-test" ]; then
      CMAKE_FLAGS+=("-DOPENITEMDEF_BUILD_TESTS=ON")
      TEST_REQUESTED=true
    elif [ "$arg" == "-btest" ]; then
      CMAKE_FLAGS+=("-DOPENITEMDEF_BUILD_TESTS=ON")
    elif [ "$arg" == "-release" ]; then
      BUILD_TYPE=Release
    elif [ "$arg" == "-cov" ]; then
      CMAKE_FLAGS+=("-DOPENITEMDEF_ENABLE_COVERAGE=ON")
      CMAKE_FLAGS+=("-DOPENITEMDEF_BUILD_TESTS=ON")
      TEST_REQUESTED=true
      COVR_REQUESTED=true
    elif [ "$arg" == "-bcov" ]; then
      CMAKE_FLAGS+=("-DOPENITEMDEF_ENABLE_COVERAGE=ON")
      CMAKE_FLAGS+=("-DOPENITEMDEF_BUILD_TESTS=ON")
    elif [ "$arg" == "-docs" ]; then
      CMAKE_FLAGS+=("-DOPENITEMDEF_BUILD_DOCS=ON")
      DOC_REQUESTED=true
    else
      OPT_FLAGS+="$arg"
    fi
done

echo ">> Building lib"

cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE "${CMAKE_FLAGS[@]}" "${OPT_FLAGS[@]}"
cmake --build "$BUILD_DIR" --config $BUILD_TYPE

if [ "$TEST_REQUESTED" = true ]; then
  echo
  echo ">> Testing lib"
  ctest --test-dir "$BUILD_DIR" --output-on-failure
fi

if [ "$COVR_REQUESTED" = true ]; then
  echo ">> Capturing coverage..."
  lcov --directory "$BUILD_DIR" --capture --output-file "$BUILD_DIR/coverage.info"
  lcov --remove "$BUILD_DIR/coverage.info" '/usr/*' '*/tests/*' -o "$BUILD_DIR/coverage.cleaned.info"
  genhtml "$BUILD_DIR/coverage.cleaned.info" --output-directory "$BUILD_DIR/coverage-report" --legend --title "libopenmodelviewer Coverage"
  echo ">> Coverage report available in: $BUILD_DIR/coverage-report/index.html"
fi

if [ "$DOC_REQUESTED" = true ]; then
  echo ">> Building Doxygen doc..."
  cmake --build .build --target docs
fi