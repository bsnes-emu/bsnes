#include <md/md.hpp>

namespace MegaDrive {

Cartridge cartridge;
#include "serialization.cpp"

auto Cartridge::hashes() const -> vector<string> {
  vector<string> hashes;
  hashes.append(information.hash);
  if(slot) for(auto& hash : slot->hashes()) hashes.append(hash);
  return hashes;
}

auto Cartridge::manifests() const -> vector<string> {
  vector<string> manifests;
  manifests.append(information.manifest);
  if(slot) for(auto& manifest : slot->manifests()) manifests.append(manifest);
  return manifests;
}

auto Cartridge::titles() const -> vector<string> {
  vector<string> titles;
  titles.append(information.title);
  if(slot) for(auto& title : slot->titles()) titles.append(title);
  return titles;
}

auto Cartridge::load() -> bool {
  unload();

  if(auto loaded = platform->load(ID::MegaDrive, "Mega Drive", "md", {"Auto", "NTSC-J", "NTSC-U", "PAL"})) {
    information.pathID = loaded.pathID;
    information.region = loaded.option;
  } else return false;

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  information.document = BML::unserialize(information.manifest);
  information.hash = information.document["game/sha256"].text();
  information.title = information.document["game/label"].text();

  if(!loadROM(rom, information.document["game/board/memory(type=ROM,content=Program)"])) {
    return unload(), false;
  }

  if(!loadROM(patch, information.document["game/board/memory(type=ROM,content=Patch)"])) {
    patch.reset();
  }

  if(!loadRAM(ram, information.document["game/board/memory(type=RAM,content=Save)"])) {
    ram.reset();
  }

  if(information.region == "Auto") {
    if(auto region = information.document["game/region"].text()) {
      information.region = region.upcase();
    } else {
      information.region = "NTSC-J";
    }
  }

  read = {&Cartridge::readLinear, this};
  write = {&Cartridge::writeLinear, this};

  if(rom.size > 0x200000) {
    read = {&Cartridge::readBanked, this};
    write = {&Cartridge::writeBanked, this};
  }

  if(patch) {
    slot = new Cartridge{depth + 1};
    if(!slot->load()) slot.reset();
    read = {&Cartridge::readLockOn, this};
    write = {&Cartridge::writeLockOn, this};
  }

  if(rom.data[0x120>>1]==0x4761 || rom.data[0x120>>1]==0x6147) {
    slot = new Cartridge{depth + 1};
    if(!slot->load()) slot.reset();
    read = {&Cartridge::readGameGenie, this};
    write = {&Cartridge::writeGameGenie, this};
  }

  //easter egg: power draw increases with each successively stacked cartridge
  //simulate increasing address/data line errors as stacking increases
  if(depth >= 3) {
    auto reader = read;
    auto writer = write;
    auto scramble = [=](uint32 value) -> uint32 {
      uint chance = max(1, (1 << 19) >> depth) - 1;
      if((random() & chance) == 0) value ^= 1 << (random() & 31);
      return value;
    };
    read = [=](uint22 address) -> uint16 {
      return scramble(reader(scramble(address)));
    };
    write = [=](uint22 address, uint16 data) -> void {
      writer(scramble(address), scramble(data));
    };
  }

  return true;
}

auto Cartridge::save() -> void {
  saveRAM(ram, information.document["game/board/memory(type=RAM,content=Save)"]);
  if(slot) slot->save();
}

auto Cartridge::unload() -> void {
  rom.reset();
  patch.reset();
  ram.reset();
  read.reset();
  write.reset();
  if(slot) slot->unload();
  slot.reset();
}

auto Cartridge::power() -> void {
  ramEnable = 1;
  ramWritable = 1;
  for(uint n : range(8)) romBank[n] = n;
  gameGenie = {};
}

//

auto Cartridge::loadROM(Memory& rom, Markup::Node memory) -> bool {
  if(!memory) return false;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  rom.size = memory["size"].natural() >> 1;
  rom.mask = bit::round(rom.size) - 1;
  rom.data = new uint16[rom.mask + 1]();
  if(auto fp = platform->open(pathID(), name, File::Read, File::Required)) {
    for(uint n : range(rom.size)) rom.data[n] = fp->readm(2);
  } else return false;

  return true;
}

auto Cartridge::loadRAM(Memory& ram, Markup::Node memory) -> bool {
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
    if(auto fp = platform->open(pathID(), name, File::Read)) {
      for(uint n : range(ram.size)) {
        if(ram.bits != 0xffff) ram.data[n] = fp->readm(1) * 0x0101;
        if(ram.bits == 0xffff) ram.data[n] = fp->readm(2);
      }
    }
  }

  return true;
}

auto Cartridge::saveRAM(Memory& ram, Markup::Node memory) -> bool {
  if(!memory) return false;
  if((bool)memory["volatile"]) return true;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  if(auto fp = platform->open(pathID(), name, File::Write)) {
    for(uint n : range(ram.size)) {
      if(ram.bits != 0xffff) fp->writem(ram.data[n], 1);
      if(ram.bits == 0xffff) fp->writem(ram.data[n], 2);
    }
  } else return false;

  return true;
}

//

auto Cartridge::readIO(uint24 address) -> uint16 {
  if(slot) slot->readIO(address);
  return 0x0000;
}

auto Cartridge::writeIO(uint24 address, uint16 data) -> void {
  if(address == 0xa130f1) ramEnable = data.bit(0), ramWritable = data.bit(1);
  if(address == 0xa130f3) romBank[1] = data;
  if(address == 0xa130f5) romBank[2] = data;
  if(address == 0xa130f7) romBank[3] = data;
  if(address == 0xa130f9) romBank[4] = data;
  if(address == 0xa130fb) romBank[5] = data;
  if(address == 0xa130fd) romBank[6] = data;
  if(address == 0xa130ff) romBank[7] = data;
  if(slot) slot->writeIO(address, data);
}

//

auto Cartridge::readLinear(uint22 address) -> uint16 {
  if(ramEnable && ram && address >= 0x200000) return ram.read(address);
  return rom.read(address);
}

auto Cartridge::writeLinear(uint22 address, uint16 data) -> void {
  //emulating ramWritable will break commercial software:
  //it does not appear that many (any?) games actually connect $a130f1.d1 to /WE;
  //hence RAM ends up always being writable, and many games fail to set d1=1
  if(ramEnable && ram && address >= 0x200000) return ram.write(address, data);
}

//

auto Cartridge::readBanked(uint22 address) -> uint16 {
  address = romBank[address.bits(19,21)] << 19 | address.bits(0,18);
  return rom.read(address);
}

auto Cartridge::writeBanked(uint22 address, uint16 data) -> void {
}

//

auto Cartridge::readLockOn(uint22 address) -> uint16 {
  if(address < 0x200000) return rom.read(address);
  if(ramEnable && address >= 0x300000) return patch.read(address);
  if(slot) return slot->read(address);
  return 0x0000;
}

auto Cartridge::writeLockOn(uint22 address, uint16 data) -> void {
  if(slot) return slot->write(address, data);
}

//

auto Cartridge::readGameGenie(uint22 address) -> uint16 {
  if(gameGenie.enable) {
    for(auto& code : gameGenie.codes) {
      if(code.enable && code.address == address) return code.data;
    }
    if(slot) return slot->read(address);
  }

  return rom.read(address);
}

auto Cartridge::writeGameGenie(uint22 address, uint16 data) -> void {
  if(gameGenie.enable) {
    if(slot) return slot->write(address, data);
  }

  if(address == 0x02 && data == 0x0001) {
    gameGenie.enable = true;
  }

  if(address >= 0x04 && address <= 0x20 && !address.bit(0)) {
    address = address - 0x04 >> 1;
    auto& code = gameGenie.codes[address / 3];
    if(address % 3 == 0) code.address.bits(16,23) = data.byte(0);
    if(address % 3 == 1) code.address.bits( 0,15) = data;
    if(address % 3 == 2) code.data = data, code.enable = true;
  }
}

//

Cartridge::Memory::operator bool() const {
  return size;
}

auto Cartridge::Memory::reset() -> void {
  delete[] data;
  data = nullptr;
  size = 0;
  mask = 0;
  bits = 0;
}

auto Cartridge::Memory::read(uint24 address) -> uint16 {
  if(!size) return 0x0000;
  return data[address >> 1 & mask];
}

auto Cartridge::Memory::write(uint24 address, uint16 word) -> void {
  if(!size) return;
  if(bits == 0x00ff) word.byte(1) = word.byte(0);
  if(bits == 0xff00) word.byte(0) = word.byte(1);
  data[address >> 1 & mask] = word;
}

}
