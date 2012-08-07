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

void Board::power() {
}

void Board::reset() {
}

void Board::serialize(serializer &s) {
  if(prgram.size) s.array(prgram.data, prgram.size);
  if(chrram.size) s.array(chrram.data, chrram.size);
}

Board::Board(XML::Document &document) {
  cartridge.board = this;
  auto &cartridge = document["cartridge"];

  information.type = cartridge["board"]["type"].data;
  information.battery = cartridge["prg"]["ram"]["nonvolatile"].data == "true";

  auto &prom = cartridge["prg"]["rom"];
  auto &pram = cartridge["prg"]["ram"];
  auto &crom = cartridge["chr"]["rom"];
  auto &cram = cartridge["chr"]["ram"];

  prgrom.size = numeral(prom["size"].data);
  prgram.size = numeral(pram["size"].data);
  chrrom.size = numeral(crom["size"].data);
  chrram.size = numeral(cram["size"].data);

  if(prgrom.size) prgrom.data = new uint8[prgrom.size]();
  if(prgram.size) prgram.data = new uint8[prgram.size]();
  if(chrrom.size) chrrom.data = new uint8[chrrom.size]();
  if(chrram.size) chrram.data = new uint8[chrram.size]();

  if(prom["name"].data) interface->loadRequest(ID::ProgramROM, prom["name"].data);
  if(pram["name"].data) interface->loadRequest(ID::ProgramRAM, pram["name"].data);
  if(crom["name"].data) interface->loadRequest(ID::CharacterROM, crom["name"].data);
  if(cram["name"].data) interface->loadRequest(ID::CharacterRAM, cram["name"].data);

  if(pram["name"].data) Famicom::cartridge.memory.append({ID::ProgramRAM, pram["name"].data});
  if(cram["name"].data) Famicom::cartridge.memory.append({ID::CharacterRAM, cram["name"].data});

  prgram.writable = true;
  chrram.writable = true;
}

Board::~Board() {
}

Board* Board::load(const string &manifest) {
  XML::Document document(manifest);
  string type = document["cartridge"]["board"]["type"].data;

  if(type == "BANDAI-FCG"  ) return new BandaiFCG(document);

  if(type == "KONAMI-VRC-1") return new KonamiVRC1(document);
  if(type == "KONAMI-VRC-2") return new KonamiVRC2(document);
  if(type == "KONAMI-VRC-3") return new KonamiVRC3(document);
  if(type == "KONAMI-VRC-4") return new KonamiVRC4(document);
  if(type == "KONAMI-VRC-6") return new KonamiVRC6(document);
  if(type == "KONAMI-VRC-7") return new KonamiVRC7(document);

  if(type == "NES-AMROM"   ) return new NES_AxROM(document);
  if(type == "NES-ANROM"   ) return new NES_AxROM(document);
  if(type == "NES-AN1ROM"  ) return new NES_AxROM(document);
  if(type == "NES-AOROM"   ) return new NES_AxROM(document);

  if(type == "NES-BNROM"   ) return new NES_BNROM(document);

  if(type == "NES-CNROM"   ) return new NES_CNROM(document);

  if(type == "NES-EKROM"   ) return new NES_ExROM(document);
  if(type == "NES-ELROM"   ) return new NES_ExROM(document);
  if(type == "NES-ETROM"   ) return new NES_ExROM(document);
  if(type == "NES-EWROM"   ) return new NES_ExROM(document);

  if(type == "NES-FJROM"   ) return new NES_FxROM(document);
  if(type == "NES-FKROM"   ) return new NES_FxROM(document);

  if(type == "NES-GNROM"   ) return new NES_GxROM(document);
  if(type == "NES-MHROM"   ) return new NES_GxROM(document);

  if(type == "NES-HKROM"   ) return new NES_HKROM(document);

  if(type == "NES-NROM-128") return new NES_NROM(document);
  if(type == "NES-NROM-256") return new NES_NROM(document);

  if(type == "NES-PEEOROM" ) return new NES_PxROM(document);
  if(type == "NES-PNROM"   ) return new NES_PxROM(document);

  if(type == "NES-SNROM"   ) return new NES_SxROM(document);
  if(type == "NES-SXROM"   ) return new NES_SxROM(document);

  if(type == "NES-TLROM"   ) return new NES_TxROM(document);

  if(type == "NES-UNROM"   ) return new NES_UxROM(document);
  if(type == "NES-UOROM"   ) return new NES_UxROM(document);

  if(type == "SUNSOFT-5B"  ) return new Sunsoft5B(document);

  return nullptr;
}
