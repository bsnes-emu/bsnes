#include <sfc/sfc.hpp>

namespace SuperFamicom {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == ID::IPLROM) {
    stream.read(smp.iplrom, min(64u, stream.size()));
  }

  if(id == ID::ROM) {
    cartridge.load(markup, stream);
    system.power();
    input.connect(0, Input::Device::Joypad);
    input.connect(1, Input::Device::Joypad);
  }

  if(id == ID::SuperGameBoyROM) {
    GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, markup, stream);
  }

  if(id == ID::BsxFlashROM) {
    bsxflash.memory.copy(stream);
  }

  if(id == ID::SufamiTurboSlotAROM) {
    sufamiturbo.slotA.rom.copy(stream);
  }

  if(id == ID::SufamiTurboSlotBROM) {
    sufamiturbo.slotB.rom.copy(stream);
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ram.data(), min(cartridge.ram.size(), stream.size()));
  }

  if(id == ID::RTC) {
    stream.read(srtc.rtc, min(stream.size(), sizeof srtc.rtc));
  }

  if(id == ID::SPC7110RTC) {
    stream.read(spc7110.rtc, min(stream.size(), sizeof srtc.rtc));
  }

  if(id == ID::BsxRAM) {
    stream.read(bsxcartridge.sram.data(), min(stream.size(), bsxcartridge.sram.size()));
  }

  if(id == ID::BsxPSRAM) {
    stream.read(bsxcartridge.psram.data(), min(stream.size(), bsxcartridge.psram.size()));
  }
}

void Interface::save(unsigned id, const stream &stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ram.data(), cartridge.ram.size());
  }

  if(id == ID::RTC) {
    stream.write(srtc.rtc, sizeof srtc.rtc);
  }

  if(id == ID::SPC7110RTC) {
    stream.write(spc7110.rtc, sizeof srtc.rtc);
  }

  if(id == ID::BsxRAM) {
    stream.write(bsxcartridge.sram.data(), bsxcartridge.sram.size());
  }

  if(id == ID::BsxPSRAM) {
    stream.write(bsxcartridge.psram.data(), bsxcartridge.psram.size());
  }
}

void Interface::unload() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.reset();
}

void Interface::run() {
  system.run();
}

void Interface::updatePalette() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;

  information.name        = "Super Famicom";
  information.width       = 256;
  information.height      = 240;
  information.aspectRatio = 8.0 / 7.0;
  information.frequency   = 32040;
  information.resettable  = true;

  information.media.append({"Super Famicom", "*.sfc"});
  information.media.append({"BS-X Satellaview", "*.bs"});
  information.media.append({"Sufami Turbo", "*.st"});
  information.media.append({"Super Game Boy", "*.gb"});

  {
    Firmware firmware;
    firmware.displayname = "Super Famicom";
    firmware.name        = "Super Famicom.sys/spc700.rom";
    firmware.id          = ID::IPLROM;
    this->firmware.append(firmware);
  }

  {
    Schema schema;
    schema.displayname = "Super Famicom";
    schema.name        = "program.rom";
    schema.filter      = "*.sfc";
    schema.id          = ID::ROM;
    this->schema.append(schema);
  }

  {
    Schema schema;
    schema.displayname = "Super Game Boy";
    schema.path        = "Super Game Boy.sfc/";
    schema.name        = "program.rom";
    schema.filter      = "*.sfc";
    schema.id          = ID::ROM;
    {
      Media slot;
      slot.displayname = "Game Boy";
      slot.name        = "program.rom";
      slot.filter      = "*.gb";
      slot.id          = ID::SuperGameBoyROM;
      schema.slot.append(schema);
    }
    this->schema.append(schema);
  }

  {
    Schema schema;
    schema.displayname = "BS-X Satellaview";
    schema.path        = "BS-X Satellaview.sfc/";
    schema.name        = "program.rom";
    schema.filter      = "*.sfc";
    schema.id          = ID::ROM;
    {
      Media slot;
      slot.displayname = "BS-X Satellaview";
      slot.name        = "program.rom";
      slot.filter      = "*.bs";
      slot.id          = ID::BsxFlashROM;
      schema.slot.append(slot);
    }
    this->schema.append(schema);
  }

  {
    Schema schema;
    schema.displayname = "Sufami Turbo";
    schema.path        = "Sufami Turbo.sfc/";
    schema.name        = "program.rom";
    schema.filter      = "*.sfc";
    schema.id          = ID::ROM;
    {
      Media slot;
      slot.displayname = "Sufami Turbo - Slot A";
      slot.name        = "program.rom";
      slot.filter      = "*.st";
      slot.id          = ID::SufamiTurboSlotAROM;
      schema.slot.append(slot);
    }
    {
      Media slot;
      slot.displayname = "Sufami Turbo - Slot B";
      slot.name        = "program.rom";
      slot.filter      = "*.st";
      slot.id          = ID::SufamiTurboSlotBROM;
      schema.slot.append(slot);
    }
    this->schema.append(schema);
  }

  {
    Port port;
    port.name = "Port 1";
    port.id   = 0;
    {
      Port::Device device;
      device.name = "None";
      device.id   = 0;
      port.device.append(device);
    }
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 1;
      device.input.append({"B",      0,  0});
      device.input.append({"Y",      0,  1});
      device.input.append({"Select", 0,  2});
      device.input.append({"Start",  0,  3});
      device.input.append({"Up",     0,  4});
      device.input.append({"Down",   0,  5});
      device.input.append({"Left",   0,  6});
      device.input.append({"Right",  0,  7});
      device.input.append({"A",      0,  8});
      device.input.append({"X",      0,  9});
      device.input.append({"L",      0, 10});
      device.input.append({"R",      0, 11});
      device.displayinput = {4, 5, 6, 7, 0, 8, 1, 9, 10, 11, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }

  {
    Port port;
    port.name = "Port 2";
    port.id   = 1;
    {
      Port::Device device;
      device.name = "None";
      device.id   = 0;
      port.device.append(device);
    }
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 1;
      device.input.append({"B",      0,  0});
      device.input.append({"Y",      0,  1});
      device.input.append({"Select", 0,  2});
      device.input.append({"Start",  0,  3});
      device.input.append({"Up",     0,  4});
      device.input.append({"Down",   0,  5});
      device.input.append({"Left",   0,  6});
      device.input.append({"Right",  0,  7});
      device.input.append({"A",      0,  8});
      device.input.append({"X",      0,  9});
      device.input.append({"L",      0, 10});
      device.input.append({"R",      0, 11});
      device.displayinput = {4, 5, 6, 7, 0, 8, 1, 9, 10, 11, 2, 3};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
