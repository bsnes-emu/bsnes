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

Board::Board(Markup::Node& document) {
  cartridge.board = this;
  auto board = document["board"];

  information.type = board["id"].text();
  information.battery = (bool)board["prg/ram/name"];

  auto prom = board["prg/rom"];
  auto pram = board["prg/ram"];
  auto crom = board["chr/rom"];
  auto cram = board["chr/ram"];

  prgrom.size = prom["size"].natural();
  prgram.size = pram["size"].natural();
  chrrom.size = crom["size"].natural();
  chrram.size = cram["size"].natural();

  if(prgrom.size) prgrom.data = new uint8_t[prgrom.size]();
  if(prgram.size) prgram.data = new uint8_t[prgram.size]();
  if(chrrom.size) chrrom.data = new uint8_t[chrrom.size]();
  if(chrram.size) chrram.data = new uint8_t[chrram.size]();

  if(prgrom.name = prom["name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), prgrom.name, File::Read, File::Required)) {
      fp->read(prgrom.data, min(prgrom.size, fp->size()));
    }
  }
  if(prgram.name = pram["name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), prgram.name, File::Read)) {
      fp->read(prgram.data, min(prgram.size, fp->size()));
    }
  }
  if(chrrom.name = crom["name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), chrrom.name, File::Read, File::Required)) {
      fp->read(chrrom.data, min(chrrom.size, fp->size()));
    }
  }
  if(chrram.name = cram["name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), chrram.name, File::Read)) {
      fp->read(chrram.data, min(chrram.size, fp->size()));
    }
  }

  prgram.writable = true;
  chrram.writable = true;
}

auto Board::save() -> void {
  auto document = BML::unserialize(cartridge.manifest());

  if(auto name = document["board/prg/ram/name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), name, File::Write)) {
      fp->write(prgram.data, prgram.size);
    }
  }

  if(auto name = document["board/chr/ram/name"].text()) {
    if(auto fp = platform->open(cartridge.pathID(), name, File::Write)) {
      fp->write(chrram.data, chrram.size);
    }
  }
}

auto Board::Memory::read(uint addr) const -> uint8 {
  return data[mirror(addr, size)];
}

auto Board::Memory::write(uint addr, uint8 byte) -> void {
  if(writable) data[mirror(addr, size)] = byte;
}

auto Board::mirror(uint addr, uint size) -> uint {
  uint base = 0;
  if(size) {
    uint mask = 1 << 23;
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

auto Board::main() -> void {
  cartridge.step(12 * 4095);
  tick();
}

auto Board::tick() -> void {
  cartridge.step(12);
  cartridge.synchronize(cpu);
}

auto Board::readCHR(uint addr) -> uint8 {
  if(chrram.size) return chrram.data[mirror(addr, chrram.size)];
  if(chrrom.size) return chrrom.data[mirror(addr, chrrom.size)];
  return 0u;
}

auto Board::writeCHR(uint addr, uint8 data) -> void {
  if(chrram.size) chrram.data[mirror(addr, chrram.size)] = data;
}

auto Board::power() -> void {
}

auto Board::serialize(serializer& s) -> void {
  if(prgram.size) s.array(prgram.data, prgram.size);
  if(chrram.size) s.array(chrram.data, chrram.size);
}

auto Board::load(string manifest) -> Board* {
  auto document = BML::unserialize(manifest);
  cartridge.information.title = document["information/title"].text();

  string type = document["board/id"].text();

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

  if(type == "NES-NROM"    ) return new NES_NROM(document);
  if(type == "NES-NROM-128") return new NES_NROM(document);
  if(type == "NES-NROM-256") return new NES_NROM(document);

  if(type == "NES-PEEOROM" ) return new NES_PxROM(document);
  if(type == "NES-PNROM"   ) return new NES_PxROM(document);

  if(type == "NES-SAROM"   ) return new NES_SxROM(document);
  if(type == "NES-SBROM"   ) return new NES_SxROM(document);
  if(type == "NES-SCROM"   ) return new NES_SxROM(document);
  if(type == "NES-SC1ROM"  ) return new NES_SxROM(document);
  if(type == "NES-SEROM"   ) return new NES_SxROM(document);
  if(type == "NES-SFROM"   ) return new NES_SxROM(document);
  if(type == "NES-SFEXPROM") return new NES_SxROM(document);
  if(type == "NES-SGROM"   ) return new NES_SxROM(document);
  if(type == "NES-SHROM"   ) return new NES_SxROM(document);
  if(type == "NES-SH1ROM"  ) return new NES_SxROM(document);
  if(type == "NES-SIROM"   ) return new NES_SxROM(document);
  if(type == "NES-SJROM"   ) return new NES_SxROM(document);
  if(type == "NES-SKROM"   ) return new NES_SxROM(document);
  if(type == "NES-SLROM"   ) return new NES_SxROM(document);
  if(type == "NES-SL1ROM"  ) return new NES_SxROM(document);
  if(type == "NES-SL2ROM"  ) return new NES_SxROM(document);
  if(type == "NES-SL3ROM"  ) return new NES_SxROM(document);
  if(type == "NES-SLRROM"  ) return new NES_SxROM(document);
  if(type == "NES-SMROM"   ) return new NES_SxROM(document);
  if(type == "NES-SNROM"   ) return new NES_SxROM(document);
  if(type == "NES-SOROM"   ) return new NES_SxROM(document);
  if(type == "NES-SUROM"   ) return new NES_SxROM(document);
  if(type == "NES-SXROM"   ) return new NES_SxROM(document);

  if(type == "NES-TBROM"   ) return new NES_TxROM(document);
  if(type == "NES-TEROM"   ) return new NES_TxROM(document);
  if(type == "NES-TFROM"   ) return new NES_TxROM(document);
  if(type == "NES-TGROM"   ) return new NES_TxROM(document);
  if(type == "NES-TKROM"   ) return new NES_TxROM(document);
  if(type == "NES-TKSROM"  ) return new NES_TxROM(document);
  if(type == "NES-TLROM"   ) return new NES_TxROM(document);
  if(type == "NES-TL1ROM"  ) return new NES_TxROM(document);
  if(type == "NES-TL2ROM"  ) return new NES_TxROM(document);
  if(type == "NES-TLSROM"  ) return new NES_TxROM(document);
  if(type == "NES-TNROM"   ) return new NES_TxROM(document);
  if(type == "NES-TQROM"   ) return new NES_TxROM(document);
  if(type == "NES-TR1ROM"  ) return new NES_TxROM(document);
  if(type == "NES-TSROM"   ) return new NES_TxROM(document);
  if(type == "NES-TVROM"   ) return new NES_TxROM(document);

  if(type == "NES-UNROM"   ) return new NES_UxROM(document);
  if(type == "NES-UOROM"   ) return new NES_UxROM(document);

  if(type == "SUNSOFT-5B"  ) return new Sunsoft5B(document);

  return nullptr;
}
