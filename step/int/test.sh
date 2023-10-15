#!/bin/sh

assert() {
  expected="$1"
  input="$2"

  ./xlang "${input}" > tmp.s

  gcc -o tmp tmp.s

  ./tmp

  actual="$?"

  rm tmp

  if [ "${actual}" = "${expected}" ]
  then
    echo "${input} => ${actual}"
  else
    echo "${input} => ${expected} expected, but got ${actual}"
    exit 1
  fi
}

assert 0 0
assert 255 255

echo "OK"
