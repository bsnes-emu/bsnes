struct PPU : Thread, MMIO {
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

  auto read(uint32 addr) -> uint8;
  auto write(uint32 addr, uint8 byte) -> void;

  auto vram_read(uint mode, uint32 addr) -> uint32;
  auto vram_write(uint mode, uint32 addr, uint32 word) -> void;

  auto pram_read(uint mode, uint32 addr) -> uint32;
  auto pram_write(uint mode, uint32 addr, uint32 word) -> void;

  auto oam_read(uint mode, uint32 addr) -> uint32;
  auto oam_write(uint mode, uint32 addr, uint32 word) -> void;

  auto render_backgrounds() -> void;
  auto render_background_linear(Registers::Background&) -> void;
  auto render_background_affine(Registers::Background&) -> void;
  auto render_background_bitmap(Registers::Background&) -> void;

  auto render_objects() -> void;
  auto render_object(Object&) -> void;
  auto object_vram_read(uint addr) const -> uint8;

  auto render_mosaic_background(uint id) -> void;
  auto render_mosaic_object() -> void;

  auto render_forceblank() -> void;
  auto render_screen() -> void;
  auto render_window(uint window) -> void;
  auto blend(uint above, uint eva, uint below, uint evb) -> uint;

  auto serialize(serializer&) -> void;

  uint8 vram[96 * 1024];
  uint16 pram[512];
  uint32* output;
};

extern PPU ppu;
