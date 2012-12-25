struct Cartridge : property<Cartridge> {
  #include "memory.hpp"

  readonly<bool> loaded;
  readonly<string> sha256;

  readonly<bool> has_sram;
  readonly<bool> has_eeprom;
  readonly<bool> has_flashrom;

  struct Information {
    string markup;
    string title;
  } information;

  string title();

  struct Media {
    unsigned id;
    string name;
  };
  vector<Media> memory;

  void load(const string &manifest);
  void unload();
  void power();

  uint8* ram_data();
  unsigned ram_size();

  uint32 read(uint8 *data, uint32 addr, uint32 size);
  void write(uint8 *data, uint32 addr, uint32 size, uint32 word);

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);

  void serialize(serializer&);
  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
