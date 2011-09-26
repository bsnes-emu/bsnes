namespace Mapper {
  struct Mapper {
    virtual void main();
    virtual void tick();

    unsigned mirror(unsigned addr, unsigned size) const;

    //note: Mapper::{prg,chr}_read() functions take literal ROM addresses; mirroring appropriately
    //subclasses of Mapper take 16-bit bus addresses; decode them; and call Mapper read functions

    virtual uint8 prg_read(unsigned addr);
    virtual void prg_write(unsigned addr, uint8 data);

    virtual uint8 chr_read(unsigned addr);
    virtual void chr_write(unsigned addr, uint8 data);

    virtual uint8 ciram_read(uint13 addr) {}
    virtual void ciram_write(uint13 addr, uint8 data) {}

    virtual unsigned ram_size();
    virtual uint8* ram_data();

    virtual void power() = 0;
    virtual void reset() = 0;

    virtual void serialize(serializer&) = 0;
  };

  #include "none/none.hpp"
  #include "aorom/aorom.hpp"
  #include "bandai-fcg/bandai-fcg.hpp"
  #include "cnrom/cnrom.hpp"
  #include "mmc1/mmc1.hpp"
  #include "mmc3/mmc3.hpp"
  #include "uorom/uorom.hpp"
  #include "vrc6/vrc6.hpp"
}
