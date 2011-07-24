synchronize() {
  if [ -d ../"$1" ]; then
    test -d "$1" && rm -r "$1"
    cp -r ../"$1" ./"$1"
  fi
}

synchronize "libco"
synchronize "nall"
synchronize "ruby"
synchronize "phoenix"

test -d libco/doc && rm -r libco/doc
test -d libco/test && rm -r libco/test
test -d nall/test && rm -r nall/test
test -d ruby/_test && rm -r ruby/_test
test -d phoenix/nall && rm -r phoenix/nall
test -d phoenix/test && rm -r phoenix/test
