BIOS::BIOS() {
  size = 16384;
  data = new uint8[size]();
}

BIOS::~BIOS() {
  delete[] data;
}

auto BIOS::read(unsigned mode, uint32 addr) -> uint32 {
  //GBA BIOS is read-protected; only the BIOS itself can read its own memory
  //when accessed elsewhere; this returns the last value read by the BIOS program
  if(cpu.r(15) >= 0x02000000) return mdr;

  if(mode & Word) return mdr = read(Half, addr &~ 2) << 0 | read(Half, addr | 2) << 16;
  if(mode & Half) return mdr = read(Byte, addr &~ 1) << 0 | read(Byte, addr | 1) <<  8;

  return mdr = data[addr & 0x3fff];
}

auto BIOS::write(unsigned mode, uint32 addr, uint32 word) -> void {
}
