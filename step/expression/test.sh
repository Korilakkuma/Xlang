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

assert "5" "1+4"
assert "2" "5-3"
assert "7" "1+4+5-3"
assert "7" "3-4+5+3"

echo "OK"
