synchronize() {
  if [ -d ../"$1" ]; then
    test -d "$1" && rm -r "$1"
    cp -r ../"$1" ./"$1"
  fi
}

synchronize "nall"
synchronize "phoenix"

test -d nall/test && rm -r nall/test
test -d phoenix/nall && rm -r phoenix/nall
test -d phoenix/test && rm -r phoenix/test
