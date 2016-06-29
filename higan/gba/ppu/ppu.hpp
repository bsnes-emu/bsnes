struct PPU : Thread, IO {
  #include "registers.hpp"
  #include "state.hpp"

  PPU();
  ~PPU();

  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;
  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto readIO(uint32 addr) -> uint8;
  auto writeIO(uint32 addr, uint8 byte) -> void;

  auto readVRAM(uint mode, uint32 addr) -> uint32;
  auto writeVRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readPRAM(uint mode, uint32 addr) -> uint32;
  auto writePRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readOAM(uint mode, uint32 addr) -> uint32;
  auto writeOAM(uint mode, uint32 addr, uint32 word) -> void;

  auto renderBackgrounds() -> void;
  auto renderBackgroundLinear(Registers::Background&) -> void;
  auto renderBackgroundAffine(Registers::Background&) -> void;
  auto renderBackgroundBitmap(Registers::Background&) -> void;

  auto renderObjects() -> void;
  auto renderObject(Object&) -> void;
  auto readObjectVRAM(uint addr) const -> uint8;

  auto renderMosaicBackground(uint id) -> void;
  auto renderMosaicObject() -> void;

  auto renderForceBlank() -> void;
  auto renderScreen() -> void;
  auto renderWindow(uint window) -> void;
  auto blend(uint above, uint eva, uint below, uint evb) -> uint;

  auto serialize(serializer&) -> void;

  uint8 vram[96 * 1024];
  uint16 pram[512];
  uint32* output;
};

extern PPU ppu;
