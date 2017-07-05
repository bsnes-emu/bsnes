BIOS bios;

BIOS::BIOS() {
  size = 16384;
  data = new uint8[size]();
}

BIOS::~BIOS() {
  delete[] data;
}

auto BIOS::read(uint mode, uint32 addr) -> uint32 {
  //unmapped memory
  if(addr >= 0x0000'4000) return cpu.pipeline.fetch.instruction;  //0000'4000-01ff'ffff

  //GBA BIOS is read-protected; only the BIOS itself can read its own memory
  //when accessed elsewhere; this should return the last value read by the BIOS program
  if(cpu.r(15) >= 0x0000'4000) return mdr;

  if(mode & Word) return mdr = read(Half, addr &~ 2) << 0 | read(Half, addr | 2) << 16;
  if(mode & Half) return mdr = read(Byte, addr &~ 1) << 0 | read(Byte, addr | 1) <<  8;

  return mdr = data[addr];
}

auto BIOS::write(uint mode, uint32 addr, uint32 word) -> void {
}
