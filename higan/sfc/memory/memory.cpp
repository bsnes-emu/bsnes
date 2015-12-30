#include <sfc/sfc.hpp>

namespace SuperFamicom {

Bus bus;

Bus::Bus() {
  lookup = new uint8 [16 * 1024 * 1024];
  target = new uint32[16 * 1024 * 1024];
}

Bus::~Bus() {
  delete[] lookup;
  delete[] target;
}

auto Bus::reset() -> void {
  function<auto (uint, uint8) -> uint8> reader = [](uint, uint8 data) { return data; };
  function<auto (uint, uint8) -> void> writer = [](uint, uint8) {};

  idcount = 0;
  map(reader, writer, 0x00, 0xff, 0x0000, 0xffff);
}

auto Bus::map() -> void {
  for(auto& m : cartridge.mapping) {
    lstring part = m.addr.split(":", 1L);
    lstring banks = part(0).split(",");
    lstring addrs = part(1).split(",");
    for(auto& bank : banks) {
      for(auto& addr : addrs) {
        lstring bankpart = bank.split("-", 1L);
        lstring addrpart = addr.split("-", 1L);
        uint banklo = hex(bankpart(0));
        uint bankhi = hex(bankpart(1, bankpart(0)));
        uint addrlo = hex(addrpart(0));
        uint addrhi = hex(addrpart(1, addrpart(0)));
        map(m.reader, m.writer, banklo, bankhi, addrlo, addrhi, m.size, m.base, m.mask);
      }
    }
  }
}

auto Bus::map(
  const function<uint8 (uint, uint8)>& reader,
  const function<void (uint, uint8)>& writer,
  uint banklo, uint bankhi, uint addrlo, uint addrhi,
  uint size, uint base, uint mask
) -> void {
  assert(banklo <= bankhi && bankhi <= 0xff);
  assert(addrlo <= addrhi && addrhi <= 0xffff);
  assert(idcount < 255);

  uint id = idcount++;
  this->reader[id] = reader;
  this->writer[id] = writer;

  for(uint bank = banklo; bank <= bankhi; bank++) {
    for(uint addr = addrlo; addr <= addrhi; addr++) {
      uint offset = reduce(bank << 16 | addr, mask);
      if(size) offset = base + mirror(offset, size - base);
      lookup[bank << 16 | addr] = id;
      target[bank << 16 | addr] = offset;
    }
  }
}

}
