namespace Mapper {
  struct Mapper {
    unsigned mirror(unsigned addr, unsigned size) const;

    virtual uint8 prg_read(uint16 addr) = 0;
    virtual void prg_write(uint16 addr, uint8 data) = 0;

    virtual uint8 chr_read(uint16 addr) = 0;
    virtual void chr_write(uint16 addr, uint8 data) = 0;

    virtual void power() = 0;
    virtual void reset() = 0;
  };

  #include "none/none.hpp"
  #include "mmc1/mmc1.hpp"
}

struct Cartridge : property<Cartridge> {
  void load(const string &xml, const uint8_t *data, unsigned size);
  void unload();

  void power();
  void reset();

  readonly<bool> loaded;

  Cartridge();

//privileged:
  Mapper::Mapper *mapper;
  Mapper::None mapperNone;
  Mapper::MMC1 mapperMMC1;

  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 *rom_data;
  unsigned rom_size;

  uint8 *prg_data;
  unsigned prg_size;

  uint8 *chr_data;
  unsigned chr_size;

  bool chr_ram;
  unsigned mirroring;
};

extern Cartridge cartridge;
