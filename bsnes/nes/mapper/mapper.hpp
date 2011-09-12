namespace Mapper {
  struct Mapper {
    unsigned mirror(unsigned addr, unsigned size) const;

    virtual uint8 prg_read(uint16 addr) = 0;
    virtual void prg_write(uint16 addr, uint8 data) = 0;

    virtual uint8 chr_read(uint16 addr) = 0;
    virtual void chr_write(uint16 addr, uint8 data) = 0;

    virtual uint8 ciram_read(uint13 addr) = 0;
    virtual void ciram_write(uint13 addr, uint8 data) = 0;

    virtual void power() = 0;
    virtual void reset() = 0;
  };

  #include "none/none.hpp"
  #include "lz93d50/lz93d50.hpp"
  #include "mmc1/mmc1.hpp"
  #include "sn74hc161n/sn74hc161n.hpp"
}
