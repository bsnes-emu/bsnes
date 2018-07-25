#include <md/md.hpp>

namespace MegaDrive {

Cartridge cartridge;
#include "serialization.cpp"

auto Cartridge::region() const -> string {
  return game.region;
}

auto Cartridge::hashes() const -> vector<string> {
  vector<string> hashes;
  hashes.append(game.hash);
  if(lockOn.hash) hashes.append(lockOn.hash);
  return hashes;
}

auto Cartridge::manifests() const -> vector<string> {
  vector<string> manifests;
  manifests.append(game.manifest);
  if(lockOn.manifest) manifests.append(lockOn.manifest);
  return manifests;
}

auto Cartridge::titles() const -> vector<string> {
  vector<string> titles;
  titles.append(game.title);
  if(lockOn.title) titles.append(lockOn.title);
  return titles;
}

auto Cartridge::load() -> bool {
  game = {};
  lockOn = {};
  read.reset();
  write.reset();

  if(!loadGame()) {
    game = {};
    return false;
  }

  read = {&Cartridge::readGame, this};
  write = {&Cartridge::writeGame, this};

  if(game.patch.size) {
    read = {&Cartridge::readLockOn, this};
    write = {&Cartridge::writeLockOn, this};
    if(!loadLockOn()) lockOn = {};
  }

  return true;
}

auto Cartridge::loadGame() -> bool {
  if(auto loaded = platform->load(ID::MegaDrive, "Mega Drive", "md", {"Auto", "NTSC-J", "NTSC-U", "PAL"})) {
    game.pathID = loaded.pathID;
    game.region = loaded.option;
  } else return false;

  if(auto fp = platform->open(game.pathID, "manifest.bml", File::Read, File::Required)) {
    game.manifest = fp->reads();
  } else return false;

  game.document = BML::unserialize(game.manifest);
  game.hash = game.document["game/sha256"].text();
  game.title = game.document["game/label"].text();

  if(!loadROM(game.rom, game.pathID, game.document["game/board/memory(type=ROM,content=Program)"])) {
    game.rom.reset();
    return false;
  }

  if(!loadROM(game.patch, game.pathID, game.document["game/board/memory(type=ROM,content=Patch)"])) {
    game.patch.reset();
  }

  if(!loadRAM(game.ram, game.pathID, game.document["game/board/memory(type=RAM,content=Save)"])) {
    game.ram.reset();
  }

  if(game.region == "Auto") {
    if(auto region = game.document["game/region"].text()) {
      game.region = region.upcase();
    } else {
      game.region = "NTSC-J";
    }
  }

  return true;
}

auto Cartridge::loadLockOn() -> bool {
  if(auto loaded = platform->load(ID::MegaDrive, "Mega Drive", "md")) {
    lockOn.pathID = loaded.pathID;
  } else return false;

  if(auto fp = platform->open(lockOn.pathID, "manifest.bml", File::Read, File::Required)) {
    lockOn.manifest = fp->reads();
  } else return false;

  lockOn.document = BML::unserialize(lockOn.manifest);
  lockOn.hash = lockOn.document["game/sha256"].text();
  lockOn.title = lockOn.document["game/label"].text();

  if(!loadROM(lockOn.rom, lockOn.pathID, lockOn.document["game/board/memory(type=ROM,content=Program)"])) {
    lockOn.rom.reset();
    return false;
  }

  if(!loadRAM(lockOn.ram, lockOn.pathID, lockOn.document["game/board/memory(type=RAM,content=Save)"])) {
    lockOn.ram.reset();
  }

  if(lockOn.rom.size >= 0x200) {
    string name;
    name.resize(48);
    for(uint n : range(24)) {
      name.get()[n * 2 + 0] = lockOn.rom.data[0x120 / 2 + n].byte(1);
      name.get()[n * 2 + 1] = lockOn.rom.data[0x120 / 2 + n].byte(0);
    }
    name.strip();
    while(name.find("  ")) name.replace("  ", " ");
    lockOn.patch = name == "SONIC THE HEDGEHOG 2";
  }

  return true;
}

auto Cartridge::loadROM(Memory& rom, uint pathID, Markup::Node memory) -> bool {
  if(!memory) return false;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  rom.size = memory["size"].natural() >> 1;
  rom.mask = bit::round(rom.size) - 1;
  rom.data = new uint16[rom.mask + 1]();
  if(auto fp = platform->open(pathID, name, File::Read, File::Required)) {
    for(uint n : range(rom.size)) rom.data[n] = fp->readm(2);
  } else return false;

  return true;
}

auto Cartridge::loadRAM(Memory& ram, uint pathID, Markup::Node memory) -> bool {
  if(!memory) return false;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  if(auto mode = memory["mode"].text()) {
    if(mode == "lo"  ) ram.bits = 0x00ff;
    if(mode == "hi"  ) ram.bits = 0xff00;
    if(mode == "word") ram.bits = 0xffff;
  }
  ram.size = memory["size"].natural() >> (ram.bits == 0xffff);
  ram.mask = bit::round(ram.size) - 1;
  ram.data = new uint16[ram.mask + 1]();
  if(!(bool)memory["volatile"]) {
    if(auto fp = platform->open(pathID, name, File::Read)) {
      for(uint n : range(ram.size)) {
        if(ram.bits != 0xffff) ram.data[n] = fp->readm(1) * 0x0101;
        if(ram.bits == 0xffff) ram.data[n] = fp->readm(2);
      }
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  saveRAM(game.ram, game.pathID, game.document["game/board/memory(type=RAM,content=Save)"]);
  saveRAM(lockOn.ram, lockOn.pathID, lockOn.document["game/board/memory(type=RAM,content=Save)"]);
}

auto Cartridge::saveRAM(Memory& ram, uint pathID, Markup::Node memory) -> bool {
  if(!memory) return false;
  if((bool)memory["volatile"]) return true;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  if(auto fp = platform->open(pathID, name, File::Write)) {
    for(uint n : range(ram.size)) {
      if(ram.bits != 0xffff) fp->writem(ram.data[n], 1);
      if(ram.bits == 0xffff) fp->writem(ram.data[n], 2);
    }
  } else return false;

  return true;
}

auto Cartridge::unload() -> void {
  game.rom.reset();
  game.patch.reset();
  game.ram.reset();
  game = {};

  lockOn.rom.reset();
  lockOn.ram.reset();
  lockOn = {};
}

auto Cartridge::power() -> void {
  ramEnable = 1;
  ramWritable = 1;
  for(auto n : range(8)) bank[n] = n;
}

//

auto Cartridge::readIO(uint24 address) -> uint16 {
  return 0x0000;
}

auto Cartridge::writeIO(uint24 address, uint16 data) -> void {
  if(address == 0xa130f1) ramEnable = data.bit(0), ramWritable = data.bit(1);
  if(address == 0xa130f3) bank[1] = data;
  if(address == 0xa130f5) bank[2] = data;
  if(address == 0xa130f7) bank[3] = data;
  if(address == 0xa130f9) bank[4] = data;
  if(address == 0xa130fb) bank[5] = data;
  if(address == 0xa130fd) bank[6] = data;
  if(address == 0xa130ff) bank[7] = data;
}

//

auto Cartridge::readGame(uint24 address) -> uint16 {
  if(address >= 0x200000 && game.ram.size && ramEnable) {
    return game.ram.data[address >> 1 & game.ram.mask];
  } else {
    address = bank[address.bits(19,21)] << 19 | address.bits(0,18);
    return game.rom.data[address >> 1 & game.rom.mask];
  }
}

auto Cartridge::writeGame(uint24 address, uint16 data) -> void {
  //emulating RAM write protect bit breaks some commercial software
  if(address >= 0x200000 && game.ram.size && ramEnable /* && ramWritable */) {
    if(game.ram.bits == 0x00ff) data = data.byte(0) * 0x0101;
    if(game.ram.bits == 0xff00) data = data.byte(1) * 0x0101;
    game.ram.data[address >> 1 & game.ram.mask] = data;
  }
}

//

auto Cartridge::readLockOn(uint24 address) -> uint16 {
  if(address >= 0x200000 && lockOn.ram.size && ramEnable) {
    return lockOn.ram.data[address >> 1 & lockOn.ram.mask];
  }

  if(address >= 0x300000 && lockOn.patch) {
    return game.patch.data[address >> 1 & game.patch.mask];
  }

  if(address >= 0x200000 && lockOn.rom.data) {
    return lockOn.rom.data[address >> 1 & lockOn.rom.mask];
  }

  if(address >= 0x200000) {
    return 0x00;
  }

  return game.rom.data[address >> 1 & game.rom.mask];
}

auto Cartridge::writeLockOn(uint24 address, uint16 data) -> void {
  if(address >= 0x200000 && lockOn.ram.size && ramEnable) {
    if(lockOn.ram.bits == 0x00ff) data = data.byte(0) * 0x0101;
    if(lockOn.ram.bits == 0xff00) data = data.byte(1) * 0x0101;
    lockOn.ram.data[address >> 1 & lockOn.ram.mask] = data;
  }
}

//

auto Cartridge::Memory::reset() -> void {
  delete[] data;
  data = nullptr;
  size = 0;
  mask = 0;
  bits = 0;
}

}
