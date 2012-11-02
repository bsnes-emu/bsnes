#include <sfc/sfc.hpp>

#define MEMORY_CPP
namespace SuperFamicom {

Bus bus;

void Bus::map(
  const function<uint8 (unsigned)> &reader,
  const function<void (unsigned, uint8)> &writer,
  unsigned banklo, unsigned bankhi,
  unsigned addrlo, unsigned addrhi,
  unsigned size, unsigned base, unsigned mask
) {
  assert(banklo <= bankhi && banklo <= 0xff);
  assert(addrlo <= addrhi && addrlo <= 0xffff);
  assert(idcount < 255);

  unsigned id = idcount++;
  this->reader[id] = reader;
  this->writer[id] = writer;

  for(unsigned bank = banklo; bank <= bankhi; bank++) {
    for(unsigned addr = addrlo; addr <= addrhi; addr++) {
      unsigned offset = reduce(bank << 16 | addr, mask);
      if(size) offset = base + mirror(offset, size - base);
      lookup[bank << 16 | addr] = id;
      target[bank << 16 | addr] = offset;
    }
  }
}

void Bus::map_reset() {
  function<uint8 (unsigned)> reader = [](unsigned) { return cpu.regs.mdr; };
  function<void (unsigned, uint8)> writer = [](unsigned, uint8) {};

  idcount = 0;
  map(reader, writer, 0x00, 0xff, 0x0000, 0xffff);
}

void Bus::map_xml() {
  for(auto &m : cartridge.mapping) {
    lstring part = m.addr.split<1>(":");
    lstring banks = part(0).split(",");
    lstring addrs = part(1).split(",");
    for(auto &bank : banks) {
      for(auto &addr : addrs) {
        lstring bankpart = bank.split<1>("-");
        lstring addrpart = addr.split<1>("-");
        unsigned banklo = hex(bankpart(0));
        unsigned bankhi = hex(bankpart(1, bankpart(0)));
        unsigned addrlo = hex(addrpart(0));
        unsigned addrhi = hex(addrpart(1, addrpart(0)));
        map(m.reader, m.writer, banklo, bankhi, addrlo, addrhi, m.size, m.base, m.mask);
      }
    }
  }
}

Bus::Bus() {
  lookup = new uint8 [16 * 1024 * 1024];
  target = new uint32[16 * 1024 * 1024];
}

Bus::~Bus() {
  delete[] lookup;
  delete[] target;
}

}
