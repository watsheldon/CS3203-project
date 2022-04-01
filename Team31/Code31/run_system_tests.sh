#!/bin/bash
#
# Searches and run all system tests located within a directory

BUILD_DIR="cmake-build-release"
OUTPUT_DIR="tests"
SOURCE_SUFFIX="_source.txt"
QUERY_SUFFIX="_queries.txt"
XML_EXT="_analysis.xml"
BOLD=$(tput bold)
NO_STYLE=$(tput sgr 0)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)

autotester="${BUILD_DIR}/src/autotester/autotester"
if [ "$(uname)" == "Windows" ]; then
  dev_null="NUL"
else
  dev_null="/dev/null"
  # For WSL
  if grep -q "microsoft" /proc/version; then
    autotester+=".exe"
  fi
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
  output="$OUTPUT_DIR/$name$XML_EXT"
  cmd="$autotester $source $queries $output"
  if $cmd &>$dev_null; then
    total_queries=$(grep -o "</query>" "$output" | wc -l)
    failed_queries=$(grep -o "</failed>" "$output" | wc -l)
    if [ "$failed_queries" -eq 0 ]; then color=$GREEN; else color=$RED; fi
    echo -e "${color}${BOLD}$((total_queries - failed_queries))/$total_queries passed${NO_STYLE}"
  else
    echo -e "${RED}${BOLD}error encountered!${NO_STYLE}"
  fi
done
