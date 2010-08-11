synchronize() {
  if [ -d ../asnes/"$1" ]; then
    test -d "$1" && rm -r "$1"
    cp -r ../asnes/"$1" ./"$1"
  fi
}

synchronize "audio"
synchronize "cartridge"
synchronize "cheat"
synchronize "chip"
synchronize "config"
synchronize "cpu/core"
synchronize "debugger"
synchronize "input"
synchronize "interface"
synchronize "libsnes"
synchronize "memory"
synchronize "ppu/counter"
synchronize "scheduler"
synchronize "smp/core"
synchronize "system"
synchronize "video"
synchronize "snes.hpp"
synchronize "Makefile"
