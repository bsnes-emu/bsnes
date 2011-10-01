#include "axrom.cpp"
#include "cnrom.cpp"
#include "nrom.cpp"
#include "sxrom.cpp"
#include "txrom.cpp"
#include "uxrom.cpp"
#include "bandai-fcg.cpp"
#include "konami-vrc6.cpp"

unsigned Board::mirror(unsigned addr, unsigned size) const {
  unsigned base = 0;
  if(size) {
    unsigned mask = 1 << 23;
    while(addr >= size) {
      while(!(addr & mask)) mask >>= 1;
      addr -= mask;
      if(size > mask) {
        size -= mask;
        base += mask;
      }
      mask >>= 1;
    }
    base += addr;
  }
  return base;
}

void Board::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    cartridge.clock += 12 * 4095;
    tick();
  }
}

void Board::tick() {
  cartridge.clock += 12;
  if(cartridge.clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

uint8 Board::prg_read(unsigned addr) {
  return prgrom.data[mirror(addr, prgrom.size)];
}

void Board::prg_write(unsigned addr, uint8 data) {
  prgrom.data[mirror(addr, prgrom.size)] = data;
}

uint8 Board::chr_read(unsigned addr) {
  if(chrrom.size) return chrrom.data[mirror(addr, chrrom.size)];
  if(chrram.size) return chrram.data[mirror(addr, chrram.size)];
  return 0u;
}

void Board::chr_write(unsigned addr, uint8 data) {
  if(chrram.size) chrram.data[mirror(addr, chrram.size)] = data;
}

Board::Memory Board::memory() {
  return prgram;
}

void Board::power() {
}

void Board::reset() {
}

void Board::serialize(serializer &s) {
  if(prgram.size) s.array(prgram.data, prgram.size);
  if(chrram.size) s.array(chrram.data, chrram.size);
}

Board::Board(BML::Node &board, const uint8_t *data, unsigned size) {
  information.type = board["type"].value();
  information.battery = board["prg"]["battery"].value();

  prgrom.size = decimal(board["prg"]["rom"].value());
  prgram.size = decimal(board["prg"]["ram"].value());
  chrrom.size = decimal(board["chr"]["rom"].value());
  chrram.size = decimal(board["chr"]["ram"].value());

  if(prgrom.size) prgrom.data = new uint8[prgrom.size]();
  if(prgram.size) prgram.data = new uint8[prgram.size]();
  if(chrrom.size) chrrom.data = new uint8[chrrom.size]();
  if(chrram.size) chrram.data = new uint8[chrram.size]();

  if(prgrom.size) memcpy(prgrom.data, data, prgrom.size);
  if(chrrom.size) memcpy(chrrom.data, data + prgrom.size, chrrom.size);
}

Board::~Board() {
  if(prgrom.size) delete[] prgrom.data;
  if(prgram.size) delete[] prgram.data;
  if(chrrom.size) delete[] chrrom.data;
  if(chrram.size) delete[] chrram.data;
}

Board* Board::load(const string &markup, const uint8_t *data, unsigned size) {
  BML::Node document(markup);
  auto &board = document["cartridge"]["board"];
  string type = board["type"].value();

  if(type == "NES-AMROM"   ) return new AxROM(board, data, size);
  if(type == "NES-ANROM"   ) return new AxROM(board, data, size);
  if(type == "NES-AN1ROM"  ) return new AxROM(board, data, size);
  if(type == "NES-AOROM"   ) return new AxROM(board, data, size);
  if(type == "NES-CNROM"   ) return new CNROM(board, data, size);
  if(type == "NES-NROM-256") return new NROM (board, data, size);
  if(type == "NES-UNROM"   ) return new UxROM(board, data, size);
  if(type == "NES-SNROM"   ) return new SxROM(board, data, size);
  if(type == "NES-SXROM"   ) return new SxROM(board, data, size);
  if(type == "NES-TLROM"   ) return new TxROM(board, data, size);
  if(type == "NES-UOROM"   ) return new UxROM(board, data, size);

  if(type == "BANDAI-FCG") return new BandaiFCG(board, data, size);

  if(type == "KONAMI-VRC-6") return new KonamiVRC6(board, data, size);

  return nullptr;
}
