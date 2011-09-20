#include <nes/nes.hpp>

namespace NES {

namespace Mapper {
  unsigned Mapper::mirror(unsigned addr, unsigned size) const {
    unsigned base = 0;
    if(size) {
      unsigned mask = 1 << 23;
      while(addr >= size) {
        while(!(addr & mask)) mask >>= 1;
        addr -= mask;
        if(size > mask) {
          size -= mask;
          base += mask;
        }
        mask >>= 1;
      }
      base += addr;
    }
    return base;
  }

  unsigned Mapper::ram_size() {
    return 0u;
  }

  uint8* Mapper::ram_data() {
    return 0;
  }

  #include "none/none.cpp"
  #include "aorom/aorom.cpp"
  #include "bandai-fcg/bandai-fcg.cpp"
  #include "cnrom/cnrom.cpp"
  #include "mmc1/mmc1.cpp"
}

}
