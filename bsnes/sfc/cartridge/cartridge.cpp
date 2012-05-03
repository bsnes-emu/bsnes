#include <sfc/sfc.hpp>

#define CARTRIDGE_CPP
namespace SuperFamicom {

#include "markup.cpp"
#include "serialization.cpp"

Cartridge cartridge;

void Cartridge::load(const string &markup, const stream &stream) {
  information.markup = markup;
  rom.copy(stream);
  sha256 = nall::sha256(rom.data(), rom.size());  //TODO: special case SGB, BSX, ST mode SHA256 sums

  region = Region::NTSC;
  ram_size = 0;

  has_gb_slot    = false;
  has_bs_cart    = false;
  has_bs_slot    = false;
  has_st_slot    = false;
  has_nss_dip    = false;
  has_superfx    = false;
  has_sa1        = false;
  has_necdsp     = false;
  has_hitachidsp = false;
  has_armdsp     = false;
  has_srtc       = false;
  has_sdd1       = false;
  has_spc7110    = false;
  has_spc7110rtc = false;
  has_obc1       = false;
  has_msu1       = false;
  has_link       = false;

  nvram.reset();

  parse_markup(markup);
//print(markup, "\n\n");

  if(ram_size > 0) {
    ram.map(allocate<uint8>(ram_size, 0xff), ram_size);
    interface->memory.append({ID::RAM, "save.ram"});
  }

  rom.write_protect(true);
  ram.write_protect(false);

  system.load();
  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;

  system.unload();
  rom.reset();
  ram.reset();

  loaded = false;
}

Cartridge::Cartridge() {
  loaded = false;
  unload();
}

Cartridge::~Cartridge() {
  unload();
}

}
