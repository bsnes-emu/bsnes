#include "chip/chip.hpp"
#include "board/board.hpp"

struct Cartridge : Thread {
  static auto Enter() -> void;
  auto main() -> void;

  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  struct Information {
    string sha256;
    string manifest;
    string title;
  } information;

//privileged:
  Board* board = nullptr;

  auto prg_read(uint addr) -> uint8;
  auto prg_write(uint addr, uint8 data) -> void;

  auto chr_read(uint addr) -> uint8;
  auto chr_write(uint addr, uint8 data) -> void;

  //scanline() is for debugging purposes only:
  //boards must detect scanline edges on their own
  auto scanline(uint y) -> void;
};

extern Cartridge cartridge;
