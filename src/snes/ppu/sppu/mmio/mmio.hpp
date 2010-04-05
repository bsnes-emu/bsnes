struct {
  uint8 ppu1_mdr;
  uint8 ppu2_mdr;

  uint16 vram_readbuffer;
  uint8 oam_latchdata;
  uint8 cgram_latchdata;
  uint8 bgofs_latchdata;
  uint8 m7_latchdata;
  bool counters_latched;
  bool latch_hcounter;
  bool latch_vcounter;

  //$2100  INIDISP
  bool display_disabled;
  unsigned display_brightness;

  //$2102  OAMADDL
  //$2103  OAMADDH
  uint16 oam_baseaddr;
  uint16 oam_addr;
  bool oam_priority;

  //$2105  BGMODE
  bool bg3_priority;
  uint8 bgmode;

  //$2115  VMAIN
  bool vram_incmode;
  uint8 vram_mapping;
  uint8 vram_incsize;

  //$2116  VMADDL
  //$2117  VMADDH
  uint16 vram_addr;

  //$211b  M7A
  uint16 m7a;

  //$211c  M7B
  uint16 m7b;

  //$211d  M7C
  uint16 m7c;

  //$211e  M7D
  uint16 m7d;

  //$211f  M7X
  uint16 m7x;

  //$2120  M7Y
  uint16 m7y;

  //$2121  CGADD
  uint16 cgram_addr;

  //$213c  OPHCT
  uint16 hcounter;

  //$213d  OPVCT
  uint16 vcounter;
} regs;

void latch_counters();
uint16 get_vram_address();

uint8 vram_read(unsigned addr);
void vram_write(unsigned addr, uint8 data);

uint8 oam_read(unsigned addr);
void oam_write(unsigned addr, uint8 data);

uint8 cgram_read(unsigned addr);
void cgram_write(unsigned addr, uint8 data);

bool interlace() const;
bool overscan() const;
bool hires() const;

void mmio_w2100(uint8);
void mmio_w2101(uint8);
void mmio_w2102(uint8);
void mmio_w2103(uint8);
void mmio_w2104(uint8);
void mmio_w2105(uint8);
void mmio_w2106(uint8);
void mmio_w2107(uint8);
void mmio_w2108(uint8);
void mmio_w2109(uint8);
void mmio_w210a(uint8);
void mmio_w210b(uint8);
void mmio_w210c(uint8);
void mmio_w210d(uint8);
void mmio_w210e(uint8);
void mmio_w210f(uint8);
void mmio_w2110(uint8);
void mmio_w2111(uint8);
void mmio_w2112(uint8);
void mmio_w2113(uint8);
void mmio_w2114(uint8);
void mmio_w2115(uint8);
void mmio_w2116(uint8);
void mmio_w2117(uint8);
void mmio_w2118(uint8);
void mmio_w2119(uint8);
void mmio_w211a(uint8);
void mmio_w211b(uint8);
void mmio_w211c(uint8);
void mmio_w211d(uint8);
void mmio_w211e(uint8);
void mmio_w211f(uint8);
void mmio_w2120(uint8);
void mmio_w2121(uint8);
void mmio_w2122(uint8);
void mmio_w2123(uint8);
void mmio_w2124(uint8);
void mmio_w2125(uint8);
void mmio_w2126(uint8);
void mmio_w2127(uint8);
void mmio_w2128(uint8);
void mmio_w2129(uint8);
void mmio_w212a(uint8);
void mmio_w212b(uint8);
void mmio_w212c(uint8);
void mmio_w212d(uint8);
void mmio_w212e(uint8);
void mmio_w212f(uint8);
void mmio_w2130(uint8);
void mmio_w2131(uint8);
void mmio_w2132(uint8);
void mmio_w2133(uint8);
uint8 mmio_r2134();
uint8 mmio_r2135();
uint8 mmio_r2136();
uint8 mmio_r2137();
uint8 mmio_r2138();
uint8 mmio_r2139();
uint8 mmio_r213a();
uint8 mmio_r213b();
uint8 mmio_r213c();
uint8 mmio_r213d();
uint8 mmio_r213e();
uint8 mmio_r213f();

void mmio_reset();
uint8 mmio_read(unsigned addr);
void mmio_write(unsigned addr, uint8 data);
