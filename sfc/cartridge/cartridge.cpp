#include <sfc/sfc.hpp>

#define CARTRIDGE_CPP
namespace SuperFamicom {

#include "markup.cpp"
#include "serialization.cpp"
Cartridge cartridge;

string Cartridge::title() {
  if(information.title.gameBoy.empty() == false) {
    return {information.title.cartridge, " + ", information.title.gameBoy};
  }

  if(information.title.satellaview.empty() == false) {
    return {information.title.cartridge, " + ", information.title.satellaview};
  }

  if(information.title.sufamiTurboA.empty() == false) {
    if(information.title.sufamiTurboB.empty() == true) {
      return {information.title.cartridge, " + ", information.title.sufamiTurboA};
    } else {
      return {information.title.cartridge, " + ", information.title.sufamiTurboA, " + ", information.title.sufamiTurboB};
    }
  }

  return information.title.cartridge;
}

void Cartridge::load() {
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

  information.markup.cartridge    = "";
  information.markup.gameBoy      = "";
  information.markup.satellaview  = "";
  information.markup.sufamiTurboA = "";
  information.markup.sufamiTurboB = "";

  information.title.cartridge     = "";
  information.title.gameBoy       = "";
  information.title.satellaview   = "";
  information.title.sufamiTurboA  = "";
  information.title.sufamiTurboB  = "";

  interface->loadRequest(ID::Manifest, "manifest.bml");
  parse_markup(information.markup.cartridge);

  //Super Game Boy
  if(cartridge.has_gb_slot()) {
    sha256 = nall::sha256(GameBoy::cartridge.romdata, GameBoy::cartridge.romsize);
  }

  //Broadcast Satellaview
  else if(cartridge.has_bs_cart() && cartridge.has_bs_slot()) {
    sha256 = nall::sha256(satellaviewcartridge.memory.data(), satellaviewcartridge.memory.size());
  }

  //Sufami Turbo
  else if(cartridge.has_st_slots()) {
    sha256_ctx sha;
    uint8_t hash[32];
    sha256_init(&sha);
    sha256_chunk(&sha, sufamiturboA.rom.data(), sufamiturboA.rom.size());
    sha256_chunk(&sha, sufamiturboB.rom.data(), sufamiturboB.rom.size());
    sha256_final(&sha);
    sha256_hash(&sha, hash);
    string result;
    for(auto& byte : hash) result.append(hex<2>(byte));
    sha256 = result;
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
    for(auto& byte : hash) result.append(hex<2>(byte));
    sha256 = result;
  }

  rom.write_protect(true);
  ram.write_protect(false);

  system.load();
  loaded = true;
}

void Cartridge::load_super_game_boy() {
  interface->loadRequest(ID::SuperGameBoyManifest, "manifest.bml");
  auto document = Markup::Document(information.markup.gameBoy);
  information.title.gameBoy = document["information/title"].text();

  auto rom = document["cartridge/rom"];
  auto ram = document["cartridge/ram"];

  GameBoy::cartridge.information.markup = information.markup.gameBoy;
  GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy);

  if(rom["name"].exists()) interface->loadRequest(ID::SuperGameBoyROM, rom["name"].data);
  if(ram["name"].exists()) interface->loadRequest(ID::SuperGameBoyRAM, ram["name"].data);
  if(ram["name"].exists()) memory.append({ID::SuperGameBoyRAM, ram["name"].data});
}

void Cartridge::load_satellaview() {
  interface->loadRequest(ID::SatellaviewManifest, "manifest.bml");
  auto document = Markup::Document(information.markup.satellaview);
  information.title.satellaview = document["information/title"].text();

  auto rom = document["cartridge/rom"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    satellaviewcartridge.memory.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SatellaviewROM, rom["name"].data);

    satellaviewcartridge.readonly = (rom["type"].text() == "MaskROM");
  }
}

void Cartridge::load_sufami_turbo_a() {
  interface->loadRequest(ID::SufamiTurboSlotAManifest, "manifest.bml");
  auto document = Markup::Document(information.markup.sufamiTurboA);
  information.title.sufamiTurboA = document["information/title"].text();

  auto rom = document["cartridge/rom"];
  auto ram = document["cartridge/ram"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    sufamiturboA.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotAROM, rom["name"].data);
  }

  if(ram["name"].exists()) {
    unsigned size = numeral(ram["size"].data);
    sufamiturboA.ram.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotARAM, ram["name"].data);
    memory.append({ID::SufamiTurboSlotARAM, ram["name"].data});
  }

  if(document["cartridge/linkable"].exists()) {
    interface->loadRequest(ID::SufamiTurboSlotB, "Sufami Turbo - Slot B", "st");
  }
}

void Cartridge::load_sufami_turbo_b() {
  interface->loadRequest(ID::SufamiTurboSlotBManifest, "manifest.bml");
  auto document = Markup::Document(information.markup.sufamiTurboB);
  information.title.sufamiTurboB = document["information/title"].text();

  auto rom = document["cartridge/rom"];
  auto ram = document["cartridge/ram"];

  if(rom["name"].exists()) {
    unsigned size = numeral(rom["size"].data);
    sufamiturboB.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotBROM, rom["name"].data);
  }

  if(ram["name"].exists()) {
    unsigned size = numeral(ram["size"].data);
    sufamiturboB.ram.map(allocate<uint8>(size, 0xff), size);
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
