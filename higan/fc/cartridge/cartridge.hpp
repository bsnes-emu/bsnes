#include "chip/chip.hpp"
#include "board/board.hpp"

struct Cartridge : Thread {
  static auto Enter() -> void;
  auto main() -> void;

  auto sha256() const -> string;
  auto manifest() const -> string;
  auto title() const -> string;

  auto load() -> void;
  auto unload() -> void;

  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  struct Information {
    string markup;
    string title;
  } information;

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

//privileged:
  Board* board = nullptr;
  string _sha256;

  auto prg_read(uint addr) -> uint8;
  auto prg_write(uint addr, uint8 data) -> void;

  auto chr_read(uint addr) -> uint8;
  auto chr_write(uint addr, uint8 data) -> void;

  //scanline() is for debugging purposes only:
  //boards must detect scanline edges on their own
  auto scanline(uint y) -> void;
};

extern Cartridge cartridge;
