void BIOS::load(const uint8 *biosdata, unsigned biossize) {
  memcpy(data, biosdata, min(size, biossize));

  string sha256 = nall::sha256(data, size);
  if(sha256 != "fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570") {
    interface->message("Warning: Game Boy Advance BIOS SHA256 sum is incorrect.");
  }
}

uint32 BIOS::read(uint32 addr, uint32 size) {
  //GBA BIOS is read-protected; only the BIOS itself can read its own memory
  //when accessed elsewhere; this returns the last value read by the BIOS program
  if(cpu.r(15) >= 0x02000000) return mdr;

  if(size == Word) return mdr = read(addr &~ 2, Half) << 0 | read(addr | 2, Half) << 16;
  if(size == Half) return mdr = read(addr &~ 1, Byte) << 0 | read(addr | 1, Byte) <<  8;
  return mdr = data[addr & 0x3fff];
}

void BIOS::write(uint32 addr, uint32 size, uint32 word) {
}

BIOS::BIOS() {
  data = new uint8[size = 16384]();
}

BIOS::~BIOS() {
  delete[] data;
}
