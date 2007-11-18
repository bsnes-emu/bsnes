void BSXCart::init() {
}

void BSXCart::enable() {
  for(uint16 i = 0x5000; i <= 0x5fff; i++) memory::mmio.map(i, *this);
}

void BSXCart::power() {
  reset();
}

void BSXCart::reset() {
  for(uint i = 0; i < 16; i++) regs.r[i] = 0x00;
  regs.r[0x07] = 0x80;
  regs.r[0x08] = 0x80;

  update_memory_map();
}

void BSXCart::update_memory_map() {
Memory &cart = (regs.r[0x01] & 0x80) == 0x00 ? (Memory&)bsxflash : (Memory&)psram;

  if((regs.r[0x02] & 0x80) == 0x00) { //LoROM mapping
    bus.map(Bus::MapLinear, 0x00, 0x7d, 0x8000, 0xffff, cart);
    bus.map(Bus::MapLinear, 0x80, 0xff, 0x8000, 0xffff, cart);
  } else { //HiROM mapping
    bus.map(Bus::MapShadow, 0x00, 0x3f, 0x8000, 0xffff, cart);
    bus.map(Bus::MapLinear, 0x40, 0x7d, 0x0000, 0xffff, cart);
    bus.map(Bus::MapShadow, 0x80, 0xbf, 0x8000, 0xffff, cart);
    bus.map(Bus::MapLinear, 0xc0, 0xff, 0x0000, 0xffff, cart);
  }

  if(regs.r[0x03] & 0x80) {
    bus.map(Bus::MapLinear, 0x60, 0x6f, 0x0000, 0xffff, psram);
  //bus.map(Bus::MapLinear, 0x70, 0x77, 0x0000, 0xffff, psram);
  }

  if((regs.r[0x05] & 0x80) == 0x00) {
    bus.map(Bus::MapLinear, 0x40, 0x4f, 0x0000, 0xffff, psram);
  }

  if((regs.r[0x06] & 0x80) == 0x00) {
    bus.map(Bus::MapLinear, 0x50, 0x5f, 0x0000, 0xffff, psram);
  }

  if(regs.r[0x07] & 0x80) {
    bus.map(Bus::MapLinear, 0x00, 0x1f, 0x8000, 0xffff, memory::cartrom);
  }

  if(regs.r[0x08] & 0x80) {
    bus.map(Bus::MapLinear, 0x80, 0x9f, 0x8000, 0xffff, memory::cartrom);
  }

  bus.map(Bus::MapShadow, 0x20, 0x3f, 0x6000, 0x7fff, psram);
  bus.map(Bus::MapLinear, 0x70, 0x77, 0x0000, 0xffff, psram);
}

uint8 BSXCart::mmio_read(uint addr) {
  if((addr & 0xf0ffff) == 0x005000) { //$[00-0f]:5000 MMIO
  uint8 n = (addr >> 16) & 15;
    return regs.r[n];
  }

  if((addr & 0xf8f000) == 0x105000) { //$[10-17]:[5000-5fff] SRAM
    return sram.read(((addr >> 16) & 7) * 0x1000 + (addr & 0xfff));
  }

  return 0x00;
}

void BSXCart::mmio_write(uint addr, uint8 data) {
  if((addr & 0xf0ffff) == 0x005000) { //$[00-0f]:5000 MMIO
  uint8 n = (addr >> 16) & 15;
    regs.r[n] = data;
    if(n == 0x0e && data & 0x80) update_memory_map();
    return;
  }

  if((addr & 0xf8f000) == 0x105000) { //$[10-17]:[5000-5fff] SRAM
    return sram.write(((addr >> 16) & 7) * 0x1000 + (addr & 0xfff), data);
  }
}

BSXCart::BSXCart() {
  sram_data  = (uint8*)malloc( 32 * 1024);
  psram_data = (uint8*)malloc(512 * 1024);

  sram.map (sram_data,   32 * 1024);
  psram.map(psram_data, 512 * 1024);
}

BSXCart::~BSXCart() {
  safe_free(sram_data);
  safe_free(psram_data);
}
