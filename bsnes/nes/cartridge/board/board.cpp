#include "bandai-fcg.cpp"
#include "konami-vrc1.cpp"
#include "konami-vrc2.cpp"
#include "konami-vrc3.cpp"
#include "konami-vrc4.cpp"
#include "konami-vrc6.cpp"
#include "konami-vrc7.cpp"
#include "nes-axrom.cpp"
#include "nes-bnrom.cpp"
#include "nes-cnrom.cpp"
#include "nes-exrom.cpp"
#include "nes-fxrom.cpp"
#include "nes-gxrom.cpp"
#include "nes-hkrom.cpp"
#include "nes-nrom.cpp"
#include "nes-pxrom.cpp"
#include "nes-sxrom.cpp"
#include "nes-txrom.cpp"
#include "nes-uxrom.cpp"
#include "sunsoft-5b.cpp"

uint8 Board::Memory::read(unsigned addr) const {
  return data[mirror(addr, size)];
}

void Board::Memory::write(unsigned addr, uint8 byte) {
  if(writable) data[mirror(addr, size)] = byte;
}

unsigned Board::mirror(unsigned addr, unsigned size) {
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

uint8 Board::chr_read(unsigned addr) {
  if(chrram.size) return chrram.data[mirror(addr, chrram.size)];
  if(chrrom.size) return chrrom.data[mirror(addr, chrrom.size)];
  return 0u;
}

void Board::chr_write(unsigned addr, uint8 data) {
  if(chrram.size) chrram.data[mirror(addr, chrram.size)] = data;
}

Board::Memory& Board::memory() {
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
  information.type = board["type"].value;
  information.battery = board["prg"]["battery"].value;

  prgrom.size = decimal(board["prg"]["rom"].value);
  prgram.size = decimal(board["prg"]["ram"].value);
  chrrom.size = decimal(board["chr"]["rom"].value);
  chrram.size = decimal(board["chr"]["ram"].value);

  if(prgrom.size) prgrom.data = new uint8[prgrom.size]();
  if(prgram.size) prgram.data = new uint8[prgram.size]();
  if(chrrom.size) chrrom.data = new uint8[chrrom.size]();
  if(chrram.size) chrram.data = new uint8[chrram.size]();

  if(prgrom.size) memcpy(prgrom.data, data, prgrom.size);
  if(chrrom.size) memcpy(chrrom.data, data + prgrom.size, chrrom.size);

  prgram.writable = true;
  chrram.writable = true;
}

Board::~Board() {
}

Board* Board::load(const string &markup, const uint8_t *data, unsigned size) {
  BML::Document document(markup);
  auto &board = document["cartridge"]["board"];
  string type = board["type"].value;

  if(type == "BANDAI-FCG") return new BandaiFCG(board, data, size);

  if(type == "KONAMI-VRC-1") return new KonamiVRC1(board, data, size);
  if(type == "KONAMI-VRC-2") return new KonamiVRC2(board, data, size);
  if(type == "KONAMI-VRC-3") return new KonamiVRC3(board, data, size);
  if(type == "KONAMI-VRC-4") return new KonamiVRC4(board, data, size);
  if(type == "KONAMI-VRC-6") return new KonamiVRC6(board, data, size);
  if(type == "KONAMI-VRC-7") return new KonamiVRC7(board, data, size);

  if(type == "NES-AMROM"   ) return new NES_AxROM(board, data, size);
  if(type == "NES-ANROM"   ) return new NES_AxROM(board, data, size);
  if(type == "NES-AN1ROM"  ) return new NES_AxROM(board, data, size);
  if(type == "NES-AOROM"   ) return new NES_AxROM(board, data, size);

  if(type == "NES-BNROM"   ) return new NES_BNROM(board, data, size);

  if(type == "NES-CNROM"   ) return new NES_CNROM(board, data, size);

  if(type == "NES-EKROM"   ) return new NES_ExROM(board, data, size);
  if(type == "NES-ELROM"   ) return new NES_ExROM(board, data, size);
  if(type == "NES-ETROM"   ) return new NES_ExROM(board, data, size);
  if(type == "NES-EWROM"   ) return new NES_ExROM(board, data, size);

  if(type == "NES-FJROM"   ) return new NES_FxROM(board, data, size);
  if(type == "NES-FKROM"   ) return new NES_FxROM(board, data, size);

  if(type == "NES-GNROM"   ) return new NES_GxROM(board, data, size);
  if(type == "NES-MHROM"   ) return new NES_GxROM(board, data, size);

  if(type == "NES-HKROM"   ) return new NES_HKROM(board, data, size);

  if(type == "NES-NROM-128") return new NES_NROM(board, data, size);
  if(type == "NES-NROM-256") return new NES_NROM(board, data, size);

  if(type == "NES-PEEOROM" ) return new NES_PxROM(board, data, size);
  if(type == "NES-PNROM"   ) return new NES_PxROM(board, data, size);

  if(type == "NES-SNROM"   ) return new NES_SxROM(board, data, size);
  if(type == "NES-SXROM"   ) return new NES_SxROM(board, data, size);

  if(type == "NES-TLROM"   ) return new NES_TxROM(board, data, size);

  if(type == "NES-UNROM"   ) return new NES_UxROM(board, data, size);
  if(type == "NES-UOROM"   ) return new NES_UxROM(board, data, size);

  if(type == "SUNSOFT-5B"  ) return new Sunsoft5B(board, data, size);

  return nullptr;
}
