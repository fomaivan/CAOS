#!/bin/sh

manpath=$MANPATH

extract_header() {
  echo $1 | sed -n -r "s/[.B ]*#include <([.a-zA-Z_0-9/]*)>/\1/p"
}

find_include() {
  IFS=':'
  for dir in $manpath
  do
    compressed="$dir/man3/$1.3.gz"
    uncompressed="$dir/man3/$1.3"

    if [ -f "$uncompressed" ]
    then
      extract_header "$(cat $uncompressed)" && return 0
    elif [ -f "$compressed" ]
    then
      extract_header "$(gunzip -c $compressed)" && return 0
    fi
  done
  return 1
}

while read line
do
  find_include $line || echo '---'
done