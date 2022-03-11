#!/usr/bin/env bash
BUILD_DIR="cmake-build-release"
OUTPUT_DIR="tests"
SOURCE_SUFFIX="_source.txt"
QUERY_SUFFIX="_queries.txt"
XML_EXT="_analysis.xml"
if [ "$(uname)" == "Windows" ]; then
  autotester="${BUILD_DIR}/src/autotester/release/autotester.exe"
  dev_null="NUL"
else
  autotester="${BUILD_DIR}/src/autotester/autotester"
  dev_null="/dev/null"
fi

if [[ -n $1 && -d $1 ]]; then
  test_dir=$1
else
  test_dir=$OUTPUT_DIR
fi
echo "System test directory: ${test_dir}"

readarray -d '' simple_sources < <(find $test_dir -type f -name "*$SOURCE_SUFFIX" -print0)
for source in "${simple_sources[@]}"; do
  queries="${source/$SOURCE_SUFFIX/$QUERY_SUFFIX}"
  if [ ! -f "$queries" ]; then
    echo "Matching file for ${source} does not exist, skipping..."
    continue
  fi
  name=$(basename "$source" $SOURCE_SUFFIX)
  echo -n "Running ${name}... "
  cmd="$autotester ${source} ${queries} $OUTPUT_DIR/$name${XML_EXT}"
  if $cmd &> $dev_null; then
    echo "success"
  else
    echo "failure"
  fi
done
