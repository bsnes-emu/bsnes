struct Regs {
  //internal
  uint8 ppu1_mdr;
  uint8 ppu2_mdr;

  uint16 vram_readbuffer;
  uint8 oam_latchdata;
  uint8 cgram_latchdata;
  uint8 mode7_latchdata;

  //$2100
  bool display_disable;
  unsigned display_brightness;

  //$2102-$2103
  uint16 oam_baseaddr;
  uint16 oam_addr;
  bool oam_priority;

  //$2115
  bool vram_incmode;
  unsigned vram_mapping;
  unsigned vram_incsize;

  //$2116-$2117
  uint16 vram_addr;

  //$211a
  unsigned mode7_repeat;
  bool mode7_vflip;
  bool mode7_hflip;

  //$211b-$2120
  uint16 m7a;
  uint16 m7b;
  uint16 m7c;
  uint16 m7d;
  uint16 m7x;
  uint16 m7y;

  //$2121
  uint16 cgram_addr;
} regs;

uint16 get_vram_addr();
uint8 vram_read(unsigned addr);
void vram_write(unsigned addr, uint8 data);

uint8 oam_read(unsigned addr);
void oam_write(unsigned addr, uint8 data);

uint8 cgram_read(unsigned addr);
void cgram_write(unsigned addr, uint8 data);

uint8 mmio_read(unsigned addr);
void mmio_write(unsigned addr, uint8 data);
void mmio_reset();
