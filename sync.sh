synchronize() {
  if [ -d ../"$1" ]; then
    test -d "$1" && rm -r "$1"
    cp -r ../"$1" ./"$1"
  fi
}

synchronize "libco"
synchronize "nall"
synchronize "ruby"

test -d libco/doc && rm -r libco/doc
test -d libco/test && rm -r libco/test
