#!/bin/bash
#
# Searches and run all system tests located within a directory

#######################################
# Display help info
# Arguments:
#   None
#######################################
function print_usage() {
  printf "Usage: %s [ OPTIONS ]\n" "$0"
  printf "   -d\t directory containing system tests\n"
  printf "   -s\t strict mode, exit on failed tests\n"
}

#######################################
# Run a single test
# Arguments:
#  Name of test case
#######################################
function run_test() {
  local queries="${1/${source_suffix}/${query_suffix}}"
  if [ ! -f "${queries}" ]; then
    echo "Matching file for ${source} does not exist, skipping..."
    return
  fi
  local name
  name=$(basename "${source}" "${source_suffix}")
  echo -n "Running ${name}... "
  local output="${output_dir}/${name}${xml_ext}"
  local cmd="${autotester} ${source} ${queries} ${output}"
  local ret=0
  if $cmd &>"$dev_null"; then
    local total_queries
    total_queries=$(grep -o "</query>" "${output}" | wc -l)
    local failed_queries
    failed_queries=$(grep -o "</failed>" "${output}" | wc -l)
    if [[ ${failed_queries} -eq 0 ]]; then
      local color=$green
    else
      color=$red
      ret=-1
    fi
    echo -e "${color}${bold}$((total_queries - failed_queries))/${total_queries} passed${no_style}"
  else
    echo -e "${red}${bold}error encountered!${no_style}"
    ret=-1
  fi
  return $ret
}

#######################################
# Main procedure to run tests
# Globals:
#   OPTARG
#   autotester
#   bold
#   dev_null
#   green
#   no_style
#   output_dir
#   query_suffix
#   red
#   source
#   source_suffix
#   xml_ext
# Arguments:
#  None
#######################################
function main() {
  output_dir="tests"
  source_suffix="_source.txt"
  query_suffix="_queries.txt"
  xml_ext="_analysis.xml"
  bold=$(tput bold)
  no_style=$(tput sgr 0)
  red=$(tput setaf 1)
  green=$(tput setaf 2)
  local build_dir="cmake-build-release"
  local strict='false'
  local test_dir=$output_dir
  local flag
  while getopts 'd:s' flag; do
    case "${flag}" in
      d)
        if [[ -n $OPTARG && -d $OPTARG ]]; then
          test_dir=$OPTARG
        fi
        ;;
      s) strict='true' ;;
      *)
        print_usage
        exit 1
        ;;
    esac
  done

  autotester="${build_dir}/src/autotester/autotester"
  if [ "$(uname)" == "Windows" ]; then
    dev_null="NUL"
  else
    dev_null="/dev/null"
    # For WSL
    if grep -q "microsoft" /proc/version; then
      autotester+=".exe"
    fi
  fi

  echo "System test directory: ${test_dir}"
  local simple_sources
  readarray -d '' simple_sources < <(find "$test_dir" -type f -name "*${source_suffix}" -print0)

  for source in "${simple_sources[@]}"; do
    run_test "$source"
    local retval=$?
    if [[ $strict == 'true' && $retval -ne 0 ]]; then
      exit 1
    fi
  done
}

main "$@"
