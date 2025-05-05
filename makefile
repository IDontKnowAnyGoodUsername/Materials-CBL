#!/bin/bash
# compile.sh

gcc "$1" -o "${1%.c}.out"

if [ $? -eq 0 ]; then
  echo "Compilation successful. Running program:"
  ./"${1%.c}.out"
else
  echo "Compilation failed."
fi