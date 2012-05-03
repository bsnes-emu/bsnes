#include <gb/gb.hpp>

namespace GameBoy {

Interface *interface = nullptr;

bool Interface::loaded() {
  return cartridge.loaded();
}

void Interface::load(unsigned id, const stream &stream, const string &markup) {
  if(id == ID::GameBoyBootROM) {
    stream.read(system.bootROM.dmg, min( 256u, stream.size()));
  }

  if(id == ID::SuperGameBoyBootROM) {
    stream.read(system.bootROM.sgb, min( 256u, stream.size()));
  }

  if(id == ID::GameBoyColorBootROM) {
    stream.read(system.bootROM.cgb, min(2048u, stream.size()));
  }

  if(id == ID::GameBoyROM) {
    cartridge.load(System::Revision::GameBoy, markup, stream);
    system.power();
  }

  if(id == ID::GameBoyColorROM) {
    cartridge.load(System::Revision::GameBoyColor, markup, stream);
    system.power();
  }

  if(id == ID::RAM) {
    stream.read(cartridge.ramdata, min(stream.size(), cartridge.ramsize));
  }
}

void Interface::save(unsigned id, const stream &stream) {
  if(id == ID::RAM) {
    stream.write(cartridge.ramdata, cartridge.ramsize);
  }
}

void Interface::unload() {
  cartridge.unload();
}

void Interface::power() {
  system.power();
}

void Interface::reset() {
  system.power();
}

void Interface::run() {
  system.run();
}

void Interface::updatePalette() {
  video.generate_palette();
}

Interface::Interface() {
  interface = this;

  information.name        = "Game Boy";
  information.width       = 160;
  information.height      = 144;
  information.aspectRatio = 1.0;
  information.frequency   = 4194304;
  information.resettable  = false;

  information.media.append({"Game Boy", "*.gb"});
  information.media.append({"Game Boy Color", "*.gbc"});

  {
    Firmware firmware;
    firmware.displayname = "Game Boy";
    firmware.name        = "Game Boy.sys/boot.rom";
    firmware.id          = ID::GameBoyBootROM;
    this->firmware.append(firmware);
  }

  {
    Firmware firmware;
    firmware.displayname = "Super Game Boy";
    firmware.name        = "Super Game Boy.sfc/boot.rom";
    firmware.id          = ID::SuperGameBoyBootROM;
    this->firmware.append(firmware);
  }

  {
    Firmware firmware;
    firmware.displayname = "Game Boy Color";
    firmware.name        = "Game Boy Color.sys/boot.rom";
    firmware.id          = ID::GameBoyColorBootROM;
    this->firmware.append(firmware);
  }

  {
    Schema schema;
    schema.displayname = "Game Boy";
    schema.name        = "program.rom";
    schema.filter      = "*.gb";
    schema.id          = ID::GameBoyROM;
    this->schema.append(schema);
  }

  {
    Schema schema;
    schema.displayname = "Game Boy Color";
    schema.name        = "program.rom";
    schema.filter      = "*.gbc";
    schema.id          = ID::GameBoyColorROM;
    this->schema.append(schema);
  }

  {
    Port port;
    port.name = "Device";
    port.id   = 0;
    {
      Port::Device device;
      device.name = "Controller";
      device.id   = 0;
      device.input.append({"Up",     0, 0});
      device.input.append({"Down",   0, 1});
      device.input.append({"Left",   0, 2});
      device.input.append({"Right",  0, 3});
      device.input.append({"B",      0, 4});
      device.input.append({"A",      0, 5});
      device.input.append({"Select", 0, 6});
      device.input.append({"Start",  0, 7});
      device.displayinput = {0, 1, 2, 3, 4, 5, 6, 7};
      port.device.append(device);
    }
    this->port.append(port);
  }
}

}
