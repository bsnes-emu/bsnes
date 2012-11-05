#include <sfc/sfc.hpp>

#define CARTRIDGE_CPP
namespace SuperFamicom {

#include "markup.cpp"
#include "serialization.cpp"
Cartridge cartridge;

void Cartridge::load(const string &manifest) {
  region = Region::NTSC;

  has_gb_slot    = false;
  has_bs_cart    = false;
  has_bs_slot    = false;
  has_st_slots   = false;
  has_nss_dip    = false;
  has_event      = false;
  has_sa1        = false;
  has_superfx    = false;
  has_armdsp     = false;
  has_hitachidsp = false;
  has_necdsp     = false;
  has_epsonrtc   = false;
  has_sharprtc   = false;
  has_spc7110    = false;
  has_sdd1       = false;
  has_obc1       = false;
  has_hsu1       = false;
  has_msu1       = false;

  this->manifest = manifest;
  parse_markup(manifest);
//print(manifest, "\n\n");

  //Super Game Boy
  if(cartridge.has_gb_slot()) {
    sha256 = nall::sha256(GameBoy::cartridge.romdata, GameBoy::cartridge.romsize);
  }

  //Broadcast Satellaview
  else if(cartridge.has_bs_cart() && cartridge.has_bs_slot()) {
    sha256 = nall::sha256(bsxflash.memory.data(), bsxflash.memory.size());
  }

  //Sufami Turbo
  else if(cartridge.has_st_slots()) {
    sha256 = nall::sha256(sufamiturbo.slotA.rom.data(), sufamiturbo.slotA.rom.size());
  }

  //Super Famicom
  else {
    sha256_ctx sha;
    uint8_t hash[32];
    vector<uint8_t> buffer;
    sha256_init(&sha);
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha256_chunk(&sha, rom.data(), rom.size());
    sha256_chunk(&sha, bsxcartridge.rom.data(), bsxcartridge.rom.size());
    sha256_chunk(&sha, sa1.rom.data(), sa1.rom.size());
    sha256_chunk(&sha, superfx.rom.data(), superfx.rom.size());
    sha256_chunk(&sha, hitachidsp.rom.data(), hitachidsp.rom.size());
    sha256_chunk(&sha, spc7110.prom.data(), spc7110.prom.size());
    sha256_chunk(&sha, spc7110.drom.data(), spc7110.drom.size());
    sha256_chunk(&sha, sdd1.rom.data(), sdd1.rom.size());
    //hash all firmware that exists
    buffer = armdsp.firmware();
    sha256_chunk(&sha, buffer.data(), buffer.size());
    buffer = hitachidsp.firmware();
    sha256_chunk(&sha, buffer.data(), buffer.size());
    buffer = necdsp.firmware();
    sha256_chunk(&sha, buffer.data(), buffer.size());
    //finalize hash
    sha256_final(&sha);
    sha256_hash(&sha, hash);
    string result;
    for(auto &byte : hash) result.append(hex<2>(byte));
    sha256 = result;
  }

  rom.write_protect(true);
  ram.write_protect(false);

  system.load();
  loaded = true;
}

void Cartridge::load_super_game_boy(const string &manifest) {
  XML::Document document(manifest);
  auto rom = document["release/cartridge/rom"];
  auto ram = document["release/cartridge/ram"];

  GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, manifest);

  if(rom["name"].exists()) interface->loadRequest(ID::SuperGameBoyROM, rom["name"].data);
  if(ram["name"].exists()) interface->loadRequest(ID::SuperGameBoyRAM, ram["name"].data);
  if(ram["name"].exists()) memory.append({ID::SuperGameBoyRAM, ram["name"].data});
}

void Cartridge::load_satellaview(const string &manifest) {
  XML::Document document(manifest);
  auto rom = document["release/cartridge/rom"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    bsxflash.memory.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::BsxFlashROM, rom["name"].data);
  }
}

void Cartridge::load_sufami_turbo_a(const string &manifest) {
  XML::Document document(manifest);
  auto rom = document["release/cartridge/rom"];
  auto ram = document["release/cartridge/ram"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    sufamiturbo.slotA.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotAROM, rom["name"].data);
  }

  if(ram["name"].exists()) {
    unsigned size = numeral(ram["size"].data);
    sufamiturbo.slotA.ram.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotARAM, ram["name"].data);
    memory.append({ID::SufamiTurboSlotARAM, ram["name"].data});
  }

  if(document["cartridge"]["linkable"].data == "true") {
    interface->loadRequest(ID::SufamiTurboSlotB, "Sufami Turbo - Slot B", "st");
  }
}

void Cartridge::load_sufami_turbo_b(const string &manifest) {
  XML::Document document(manifest);
  auto rom = document["release/cartridge/rom"];
  auto ram = document["release/cartridge/ram"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    sufamiturbo.slotB.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotBROM, rom["name"].data);
  }

  if(ram["name"].exists()) {
    unsigned size = numeral(ram["size"].data);
    sufamiturbo.slotB.ram.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotBRAM, ram["name"].data);
    memory.append({ID::SufamiTurboSlotBRAM, ram["name"].data});
  }
}

void Cartridge::unload() {
  if(loaded == false) return;

  system.unload();
  rom.reset();
  ram.reset();

  loaded = false;
  memory.reset();
}

Cartridge::Cartridge() {
  loaded = false;
}

Cartridge::~Cartridge() {
  unload();
}

}
