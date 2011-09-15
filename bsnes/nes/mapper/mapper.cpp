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

  #include "none/none.cpp"
  #include "aorom/aorom.cpp"
  #include "bandai-fcg/bandai-fcg.cpp"
  #include "cnrom/cnrom.cpp"
  #include "mmc1/mmc1.cpp"
}

}
