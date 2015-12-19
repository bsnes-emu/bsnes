#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "markup.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::manifest() -> string {
  string manifest = information.markup.cartridge;

  if(information.markup.gameBoy) {
    manifest.append("\n[Super Game Boy]\n");
    manifest.append(information.markup.gameBoy);
  }

  if(information.markup.satellaview) {
    manifest.append("\n[BS-X Satellaview]\n");
    manifest.append(information.markup.satellaview);
  }

  if(information.markup.sufamiTurboA) {
    manifest.append("\n[Sufami Turbo - Slot A]\n");
    manifest.append(information.markup.sufamiTurboA);
  }

  if(information.markup.sufamiTurboB) {
    manifest.append("\n[Sufami Turbo - Slot B]\n");
    manifest.append(information.markup.sufamiTurboB);
  }

  return manifest;
}

auto Cartridge::title() -> string {
  string title = information.title.cartridge;

  if(information.title.gameBoy) {
    title.append(" + ", information.title.gameBoy);
  }

  if(information.title.satellaview) {
    title.append(" + ", information.title.satellaview);
  }

  if(information.title.sufamiTurboA) {
    title.append(" + ", information.title.sufamiTurboA);
  }

  if(information.title.sufamiTurboB) {
    title.append(" + ", information.title.sufamiTurboB);
  }

  return title;
}

auto Cartridge::load() -> void {
  _region = Region::NTSC;

  hasICD2       = false;
  hasMCC        = false;
  hasNSSDIP     = false;
  hasEvent      = false;
  hasSA1        = false;
  hasSuperFX    = false;
  hasARMDSP     = false;
  hasHitachiDSP = false;
  hasNECDSP     = false;
  hasEpsonRTC   = false;
  hasSharpRTC   = false;
  hasSPC7110    = false;
  hasSDD1       = false;
  hasOBC1       = false;
  hasMSU1       = false;

  hasSuperGameBoySlot = false;
  hasSatellaviewSlot  = false;
  hasSufamiTurboSlots = false;

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

  interface->loadRequest(ID::Manifest, "manifest.bml", true);
  parseMarkup(information.markup.cartridge);

  //Super Game Boy
  if(cartridge.hasICD2()) {
    _sha256 = Hash::SHA256(GameBoy::cartridge.romdata, GameBoy::cartridge.romsize).digest();
  }

  //Broadcast Satellaview
  else if(cartridge.hasMCC() && cartridge.hasSatellaviewSlot()) {
    _sha256 = Hash::SHA256(satellaviewcartridge.memory.data(), satellaviewcartridge.memory.size()).digest();
  }

  //Sufami Turbo
  else if(cartridge.hasSufamiTurboSlots()) {
    Hash::SHA256 sha;
    sha.data(sufamiturboA.rom.data(), sufamiturboA.rom.size());
    sha.data(sufamiturboB.rom.data(), sufamiturboB.rom.size());
    _sha256 = sha.digest();
  }

  //Super Famicom
  else {
    Hash::SHA256 sha;
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha.data(rom.data(), rom.size());
    sha.data(mcc.rom.data(), mcc.rom.size());
    sha.data(sa1.rom.data(), sa1.rom.size());
    sha.data(superfx.rom.data(), superfx.rom.size());
    sha.data(hitachidsp.rom.data(), hitachidsp.rom.size());
    sha.data(spc7110.prom.data(), spc7110.prom.size());
    sha.data(spc7110.drom.data(), spc7110.drom.size());
    sha.data(sdd1.rom.data(), sdd1.rom.size());
    //hash all firmware that exists
    vector<uint8> buffer;
    buffer = armdsp.firmware();
    sha.data(buffer.data(), buffer.size());
    buffer = hitachidsp.firmware();
    sha.data(buffer.data(), buffer.size());
    buffer = necdsp.firmware();
    sha.data(buffer.data(), buffer.size());
    //finalize hash
    _sha256 = sha.digest();
  }

  rom.write_protect(true);
  ram.write_protect(false);

  system.load();
  _loaded = true;
}

auto Cartridge::loadSuperGameBoy() -> void {
  interface->loadRequest(ID::SuperGameBoyManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.markup.gameBoy);
  information.title.gameBoy = document["information/title"].text();

  auto rom = document["board/rom"];
  auto ram = document["board/ram"];

  GameBoy::cartridge.information.markup = information.markup.gameBoy;
  GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy);

  if(auto name = rom["name"].text()) interface->loadRequest(ID::SuperGameBoyROM, name, true);
  if(auto name = ram["name"].text()) interface->loadRequest(ID::SuperGameBoyRAM, name, false);
  if(auto name = ram["name"].text()) memory.append({ID::SuperGameBoyRAM, name});
}

auto Cartridge::loadSatellaview() -> void {
  interface->loadRequest(ID::SatellaviewManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.markup.satellaview);
  information.title.satellaview = document["information/title"].text();

  auto rom = document["board/rom"];

  if(rom["name"]) {
    unsigned size = rom["size"].natural();
    satellaviewcartridge.memory.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SatellaviewROM, rom["name"].text(), true);

    satellaviewcartridge.readonly = (rom["type"].text() == "MaskROM");
  }
}

auto Cartridge::loadSufamiTurboA() -> void {
  interface->loadRequest(ID::SufamiTurboSlotAManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.markup.sufamiTurboA);
  information.title.sufamiTurboA = document["information/title"].text();

  auto rom = document["board/rom"];
  auto ram = document["board/ram"];

  if(rom["name"]) {
    unsigned size = rom["size"].natural();
    sufamiturboA.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotAROM, rom["name"].text(), true);
  }

  if(ram["name"]) {
    unsigned size = ram["size"].natural();
    sufamiturboA.ram.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotARAM, ram["name"].text(), false);
    memory.append({ID::SufamiTurboSlotARAM, ram["name"].text()});
  }

  if(document["board/linkable"]) {
    interface->loadRequest(ID::SufamiTurboSlotB, "Sufami Turbo - Slot B", "st", false);
  }
}

auto Cartridge::loadSufamiTurboB() -> void {
  interface->loadRequest(ID::SufamiTurboSlotBManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.markup.sufamiTurboB);
  information.title.sufamiTurboB = document["information/title"].text();

  auto rom = document["board/rom"];
  auto ram = document["board/ram"];

  if(rom["name"]) {
    unsigned size = rom["size"].natural();
    sufamiturboB.rom.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotBROM, rom["name"].text(), true);
  }

  if(ram["name"]) {
    unsigned size = ram["size"].natural();
    sufamiturboB.ram.map(allocate<uint8>(size, 0xff), size);
    interface->loadRequest(ID::SufamiTurboSlotBRAM, ram["name"].text(), false);
    memory.append({ID::SufamiTurboSlotBRAM, ram["name"].text()});
  }
}

auto Cartridge::unload() -> void {
  if(_loaded) {
    system.unload();
    rom.reset();
    ram.reset();

    _loaded = false;
    memory.reset();
  }
}

}
