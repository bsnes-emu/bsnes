class bROM : public ROM {
private:
bool write_protection;
bool rom_loaded;

public:
enum { LOROM = 0, HIROM = 1 };
uint8  *data, *sram_data;
uint8  mapper;
uint32 size, sram_size;
  void  load_rom(Reader *rf);
  void  load_sram(Reader *rf);
  void  save_sram(Writer *wf);
  void  unload();
  uint8 read (uint32 addr);
  void  write(uint32 addr, byte value);
  void  write_protect(bool yn);

  bROM();
  ~bROM();
};

class bMemBus : public MemBus {
public:
byte *wram;
  uint8 read (uint32 addr);
  void  write(uint32 addr, byte value);

  void power();
  void reset();

  bMemBus();
  ~bMemBus();
};
