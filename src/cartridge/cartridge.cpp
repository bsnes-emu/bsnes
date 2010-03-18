#include <../base.hpp>
#include <nall/crc32.hpp>
#include <nall/sha256.hpp>

#define CARTRIDGE_CPP
namespace SNES {

#include "xml.cpp"
#include "serialization.cpp"

namespace memory {
  MappedRAM cartrom, cartram, cartrtc;
  MappedRAM bsxflash, bsxram, bsxpram;
  MappedRAM stArom, stAram;
  MappedRAM stBrom, stBram;
  MappedRAM gbrom, gbram, gbrtc;
};

Cartridge cartridge;

void Cartridge::load(Mode cartridge_mode, const lstring &xml_list) {
  mode = Mode::Normal;
  region = Region::NTSC;
  ram_size = 0;
  spc7110_data_rom_offset = 0x100000;
  supergameboy_version = SuperGameBoyVersion::Version1;
  supergameboy_ram_size = 0;
  supergameboy_rtc_size = 0;

  has_bsx_slot   = false;
  has_superfx    = false;
  has_sa1        = false;
  has_srtc       = false;
  has_sdd1       = false;
  has_spc7110    = false;
  has_spc7110rtc = false;
  has_cx4        = false;
  has_dsp1       = false;
  has_dsp2       = false;
  has_dsp3       = false;
  has_dsp4       = false;
  has_obc1       = false;
  has_st0010     = false;
  has_st0011     = false;
  has_st0018     = false;
  has_msu1       = false;

  mode = cartridge_mode;
  parse_xml(xml_list);

  if(ram_size > 0) {
    memory::cartram.map(allocate<uint8_t>(ram_size, 0xff), ram_size);
  }

  if(has_srtc || has_spc7110rtc) {
    memory::cartrtc.map(allocate<uint8_t>(20, 0xff), 20);
  }

  if(mode == Mode::Bsx) {
    memory::bsxram.map (allocate<uint8_t>( 32 * 1024, 0xff),  32 * 1024);
    memory::bsxpram.map(allocate<uint8_t>(512 * 1024, 0xff), 512 * 1024);
  }

  if(mode == Mode::SufamiTurbo) {
    if(memory::stArom.data()) memory::stAram.map(allocate<uint8_t>(128 * 1024, 0xff), 128 * 1024);
    if(memory::stBrom.data()) memory::stBram.map(allocate<uint8_t>(128 * 1024, 0xff), 128 * 1024);
  }

  if(mode == Mode::SuperGameBoy) {
    if(memory::gbrom.data()) {
      if(supergameboy_ram_size) memory::gbram.map(allocate<uint8_t>(supergameboy_ram_size, 0xff), supergameboy_ram_size);
      if(supergameboy_rtc_size) memory::gbrtc.map(allocate<uint8_t>(supergameboy_rtc_size, 0x00), supergameboy_rtc_size);
    }
  }

  memory::cartrom.write_protect(true);
  memory::cartram.write_protect(false);
  memory::cartrtc.write_protect(false);
  memory::bsxflash.write_protect(true);
  memory::bsxram.write_protect(false);
  memory::bsxpram.write_protect(false);
  memory::stArom.write_protect(true);
  memory::stAram.write_protect(false);
  memory::stBrom.write_protect(true);
  memory::stBram.write_protect(false);
  memory::gbrom.write_protect(true);
  memory::gbram.write_protect(false);
  memory::gbrtc.write_protect(false);

  unsigned checksum = ~0;                                           foreach(n, memory::cartrom ) checksum = crc32_adjust(checksum, n);
  if(memory::bsxflash.size() != 0 && memory::bsxflash.size() != ~0) foreach(n, memory::bsxflash) checksum = crc32_adjust(checksum, n);
  if(memory::stArom.size()   != 0 && memory::stArom.size()   != ~0) foreach(n, memory::stArom  ) checksum = crc32_adjust(checksum, n);
  if(memory::stBrom.size()   != 0 && memory::stBrom.size()   != ~0) foreach(n, memory::stBrom  ) checksum = crc32_adjust(checksum, n);
  if(memory::gbrom.size()    != 0 && memory::gbrom.size()    != ~0) foreach(n, memory::gbrom   ) checksum = crc32_adjust(checksum, n);
  crc32 = ~checksum;

  sha256_ctx sha;
  uint8_t shahash[32];
  sha256_init(&sha);
  sha256_chunk(&sha, memory::cartrom.data(), memory::cartrom.size());
  sha256_final(&sha);
  sha256_hash(&sha, shahash);

  string hash;
  foreach(n, shahash) hash << strhex<2>(n);
  sha256 = hash;

  bus.load_cart();
  system.serialize_init();
  loaded = true;
}

void Cartridge::unload() {
  memory::cartrom.reset();
  memory::cartram.reset();
  memory::cartrtc.reset();
  memory::bsxflash.reset();
  memory::bsxram.reset();
  memory::bsxpram.reset();
  memory::stArom.reset();
  memory::stAram.reset();
  memory::stBrom.reset();
  memory::stBram.reset();
  memory::gbrom.reset();
  memory::gbram.reset();
  memory::gbrtc.reset();

  if(loaded == false) return;
  bus.unload_cart();
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
