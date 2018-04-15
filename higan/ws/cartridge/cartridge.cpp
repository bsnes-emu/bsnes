#include <ws/ws.hpp>

namespace WonderSwan {

Cartridge cartridge;
#include "memory.cpp"
#include "rtc.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto Cartridge::Enter() -> void {
  while(true) scheduler.synchronize(), cartridge.main();
}

auto Cartridge::main() -> void {
  if(rtc.data) {
    rtcTickSecond();
    rtcCheckAlarm();
  }
  step(3'072'000);
}

auto Cartridge::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto Cartridge::power() -> void {
  create(Cartridge::Enter, 3'072'000);
  eeprom.power();

  bus.map(this, 0x00c0, 0x00c8);
  if(rtc.data) bus.map(this, 0x00ca, 0x00cb);
  bus.map(this, 0x00cc, 0x00cd);

  memory::fill(&r, sizeof(Registers));

  r.romBank0 = 0xff;
  r.romBank1 = 0xff;
  r.romBank2 = 0xff;
  r.sramBank = 0xff;
}

auto Cartridge::load() -> bool {
  information = {};

  if(Model::WonderSwan()) {
    if(auto loaded = platform->load(ID::WonderSwan, "WonderSwan", "ws")) {
      information.pathID = loaded.pathID();
    } else return false;
  }

  if(Model::WonderSwanColor() || Model::SwanCrystal()) {
    if(auto loaded = platform->load(ID::WonderSwanColor, "WonderSwan Color", "wsc")) {
      information.pathID = loaded.pathID();
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=ROM,content=Program)"]}) {
    rom.size = memory.size;
    rom.mask = bit::round(rom.size) - 1;
    rom.data = new uint8[rom.mask + 1];
    memory::fill(rom.data, rom.mask + 1, 0xff);
    if(auto fp = platform->open(pathID(), memory.name(), File::Read, File::Required)) {
      fp->read(rom.data, rom.size);
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    ram.size = memory.size;
    ram.mask = bit::round(ram.size) - 1;
    ram.data = new uint8[ram.mask + 1];
    memory::fill(ram.data, ram.mask + 1, 0xff);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
        fp->read(ram.data, ram.size);
      }
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    eeprom.setSize(memory.size / sizeof(uint16));
    eeprom.erase();
    if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
      fp->read(eeprom.data(), eeprom.size());
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    rtc.size = memory.size;
    rtc.mask = bit::round(rtc.size) - 1;
    rtc.data = new uint8[rtc.mask + 1];
    memory::fill(rtc.data, rtc.mask + 1, 0x00);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
        fp->read(rtc.data, rtc.size);
      }
    }
  }

  information.title = document["game/label"].text();
  information.orientation = document["game/orientation"].text() == "vertical";
  information.sha256 = Hash::SHA256(rom.data, rom.size).digest();
  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
        fp->write(ram.data, ram.size);
      }
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
      fp->write(eeprom.data(), eeprom.size());
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
        fp->write(rtc.data, rtc.size);
      }
    }
  }
}

auto Cartridge::unload() -> void {
  delete[] rom.data;
  rom.data = nullptr;
  rom.size = 0;
  rom.mask = 0;

  delete[] ram.data;
  ram.data = nullptr;
  ram.size = 0;
  ram.mask = 0;

  delete[] rtc.data;
  rtc.data = nullptr;
  rtc.size = 0;
  rtc.mask = 0;
}

}
