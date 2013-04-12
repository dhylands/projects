# Function to translate NS_ERROR codes

nserror() {
  # find an error name based on it's code, or vice-versa using xpcshell
  XUL_DIR=/home/work/B2G-profiler/gaia/xulrunner-sdk/bin
  XPCSHELL=${XUL_DIR}/xpcshell
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${XUL_DIR}"
  if [[ -z $1 ]]; then
    echo "Error: required error code or name missing"
    return 1
  fi

  ERR=$1
  if [[ "$ERR" = NS_* ]]; then
    $XPCSHELL -e "print(\"$ERR = \" + Components.results.$ERR);"
  else
    if (( $ERR < 0 )); then
      ERR=$(( 4294967296 + $ERR ))
    fi
    $XPCSHELL -e "for (var err in Components.results) { var code = Components.results[err]; if (code === $ERR) { print(err + \" = \" + code); } }"
  fi
}
