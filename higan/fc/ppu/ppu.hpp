#include "video.hpp"

struct PPU : Thread {
  static auto Main() -> void;
  auto main() -> void;
  auto tick() -> void;

  auto scanline() -> void;
  auto frame() -> void;

  auto power() -> void;
  auto reset() -> void;

  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;

  auto ciram_read(uint16 addr) -> uint8;
  auto ciram_write(uint16 addr, uint8 data) -> void;

  auto cgram_read(uint16 addr) -> uint8;
  auto cgram_write(uint16 addr, uint8 data) -> void;

  auto raster_enable() const -> bool;
  auto nametable_addr() const -> uint;
  auto scrollx() const -> uint;
  auto scrolly() const -> uint;
  auto sprite_height() const -> uint;

  auto chr_load(uint16 addr) -> uint8;

  auto scrollx_increment() -> void;
  auto scrolly_increment() -> void;

  auto raster_pixel() -> void;
  auto raster_sprite() -> void;
  auto raster_scanline() -> void;

  auto serialize(serializer&) -> void;

  struct Status {
    uint8 mdr;

    bool field;
    uint lx;
    uint ly;

    uint8 bus_data;

    bool address_latch;

    uint15 vaddr;
    uint15 taddr;
    uint8 xaddr;

    bool nmi_hold;
    bool nmi_flag;

    //$2000
    bool nmi_enable;
    bool master_select;
    bool sprite_size;
    uint bg_addr;
    uint sprite_addr;
    uint vram_increment;

    //$2001
    uint3 emphasis;
    bool sprite_enable;
    bool bg_enable;
    bool sprite_edge_enable;
    bool bg_edge_enable;
    bool grayscale;

    //$2002
    bool sprite_zero_hit;
    bool sprite_overflow;

    //$2003
    uint8 oam_addr;
  } status;

  struct Raster {
    uint16 nametable;
    uint16 attribute;
    uint16 tiledatalo;
    uint16 tiledatahi;

    uint oam_iterator;
    uint oam_counter;

    struct OAM {
      uint8 id;
      uint8 y;
      uint8 tile;
      uint8 attr;
      uint8 x;

      uint8 tiledatalo;
      uint8 tiledatahi;
    } oam[8], soam[8];
  } raster;

  uint32 buffer[256 * 262];
  uint8 ciram[2048];
  uint8 cgram[32];
  uint8 oam[256];
};

extern PPU ppu;
