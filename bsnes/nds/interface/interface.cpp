#include <nds/nds.hpp>
#include <sys/time.h>

namespace NintendoDS {

Interface *interface = nullptr;

double Interface::videoFrequency() {
  return 2.*33513982 / (2*6 * 263*355);
}

double Interface::audioFrequency() {
  return 2.*33513982 / (2 * 1024);
}

bool Interface::loaded() {
  return true;
}

unsigned Interface::group(unsigned id) {
  if(id == ARM7BIOS  || id == ARM9BIOS)    return 0;
  if(id == Firmware  || id == Clock)       return 0;
  
  if(id == Slot1ROM  || id == Slot1EEPROM) return 1;
  if(id == Slot1FRAM || id == Slot1Flash)  return 1;
  
  if(id == Slot2ROM  || id == Slot2RAM)    return 2;
  if(id == Slot2SRAM || id == Slot2EEPROM) return 2;
  if(id == Slot2FRAM || id == Slot2Flash)  return 2;
  
  return 0;
}

void Interface::load(unsigned id, const string& manifest) {
  //print(manifest,"\n");
  string syspath = interface->path(System);
  
  if(id == NintendoDS) {
    gameManifest = manifest;
    systemManifest.readfile({syspath, "manifest.xml"});
    XML::Document sysdoc(systemManifest);
    
    if(!sysdoc["system"].exists()) {
      interface->notify("manifest.xml not found");
    }
    else {
      auto &sys = sysdoc["system"];
      
      string arm7BiosFile = sys["arm7"]["bios"]["data"].data;
      string arm9BiosFile = sys["arm9"]["bios"]["data"].data;
      string firmwareFile = sys["flash"]["data"].data;
      string clockXmlFile = sys["rtc"]["data"].data;
      
      if(!file::exists({syspath, arm7BiosFile})) interface->notify("arm7 bios not found");
      if(!file::exists({syspath, arm9BiosFile})) interface->notify("arm9 bios not found");
      if(!file::exists({syspath, firmwareFile})) interface->notify("firmware not found");
      
      interface->loadRequest(ARM7BIOS, arm7BiosFile);
      interface->loadRequest(ARM9BIOS, arm9BiosFile);
      interface->loadRequest(Firmware, firmwareFile);
      interface->loadRequest(Clock,    clockXmlFile);
    }
    
    if(gameManifest == "") {
      // Default to 1GB ROM with no save. Since GameCard does bounds-check,
      // we only allocate enough to hold the stream passed in.
      gameManifest =
        "<cartridge>"
          "<slot1>"
            "<rom name=\"rom\" size=\"0x40000000\" />"
          "</slot1>"
        "</cartridge>";
    }
    
    // <cartridge>
    XML::Document document(gameManifest);
    
    //if(document.error != "") {
    //  print(document.error,"\n");
    //  return;
    //}
    
    auto &eslot1 = document["cartridge"]["slot1"];
    
    // <slot1>
    if(eslot1.exists()) {
      // <rom name=.. id=.. size=.. sha256=.. />
      if(eslot1["rom"].exists()) {
        string file   = string(eslot1["rom"]["name"].data);
        uint32 size   = numeral(eslot1["rom"]["size"].data);
        uint32 chipId = numeral(eslot1["rom"]["id"].data);
        
        print("Loading slot-1 ROM (", file, ").. ");
        slot1.load(new GameCard(chipId));
        interface->loadRequest(Slot1ROM, file);
        print("\n");
      }
      // <save name=.. type=EEPROM,FRAM,Flash size=.. [page|id=..] />
      if(eslot1["save"].exists()) {
        string file   = string(eslot1["save"]["name"].data);
        string type   = string(eslot1["save"]["type"].data);
        uint32 size   = numeral(eslot1["save"]["size"].data);
        uint32 psize  = numeral(eslot1["save"]["page"].data);  // EEPROM only
        uint32 chipId = numeral(eslot1["save"]["id"].data);    // Flash only
        
        unsigned id = 0;
        if(auto card = slot1.card) {
          if(type == "EEPROM") id = Slot1EEPROM, card->spi = new EEPROM(size, psize);
          if(type == "Flash")  id = Slot1Flash,  card->spi = new Flash(size, chipId);
          if(type == "FRAM")   id = Slot1FRAM,   card->spi = new FRAM(size);
        }
        if(id) {
          print("Loading slot-1 ",eslot1["save"]["type"].data," (", file, ").. ");
          interface->loadRequest(id, file);
          print("\n");
        }
      }
      // <irport />
      if(eslot1["irport"].exists()) {
        // Required by Pokemon HG/SS and B/W. These cards have an infrared port
        // built-in. Since there's only one /CS, access to flash memory passes
        // through the infrared bridge via an override command.
        slot1.card->spi = new IRPort(slot1.card->spi);
      }
    }
  }
  
  // Provide blank images if needed (we'd crash otherwise).
  if(system.firmware.size == 0) {
    system.firmware.size = 0x40000;
    system.firmware.data = new uint8[system.firmware.size];
    memset(system.firmware.data, 0xff, system.firmware.size);
  }
  if(arm7.bios.size == 0) {
    arm7.bios.size = 4;
    arm7.bios.data = new uint32[arm7.bios.size/4];
    memset(arm7.bios.data, 0xef, arm7.bios.size);
  }
  if(arm9.bios.size == 0) {
    arm9.bios.size = 4;
    arm9.bios.data = new uint32[arm9.bios.size/4];
    memset(arm9.bios.data, 0xef, arm9.bios.size);
  }
}

void Interface::load(unsigned id, const stream& memory, const string& markup) {
  if(id == ARM7BIOS) return system.loadArm7Bios(memory);
  if(id == ARM9BIOS) return system.loadArm9Bios(memory);
  if(id == Firmware) return system.loadFirmware(memory);
  if(id == Clock)    return system.loadRTC(memory);
  
  XML::Document document(gameManifest);
  
  auto &eslot1 = document["cartridge"]["slot1"];
  
  if(eslot1.exists()) {
    if(eslot1["rom"].exists() && id == Slot1ROM) {
      string hash = string(eslot1["rom"]["sha256"].data);
      uint32 size = numeral(eslot1["rom"]["size"].data);
      
      if(auto card = slot1.card) {
        delete card->rom.data;
        card->rom.size = min(memory.size(), size);
        card->rom.data = new uint8[card->rom.size];
        card->size     = bit::round(size);
        
        memory.read(card->rom.data, card->rom.size);
        
        if(hash && hash != card->sha256) print("SHA256 mismatch.");
        else                             print("OK.");
      }
    }
    if(eslot1["save"].exists() && slot1.card && (id==Slot1EEPROM || id==Slot1Flash || id==Slot1FRAM)) {
      uint32 size = numeral(eslot1["save"]["size"].data);
      
      if(auto save = slot1.card->spi) {
        if(auto irport = dynamic_cast<IRPort*>(save))
          save = irport->slave;
        
        if(auto media = dynamic_cast<StaticMemory*>(save)) {
          memory.read(media->data, min(media->size, memory.size()));
          print("OK.");
        }
      }
    }
  }
}



void Interface::save() {
  XML::Document sysdoc(systemManifest);
  
  if(sysdoc["system"].exists()) {
    auto &sys = sysdoc["system"];
    
    interface->saveRequest(Firmware, sys["flash"]["data"].data);
    interface->saveRequest(Clock,    sys["rtc"]["data"].data);
  }
  
  XML::Document document(gameManifest);
  auto &eslot1 = document["cartridge"]["slot1"];
  
  if(eslot1.exists() && eslot1["save"].exists()) {
    string file = eslot1["save"]["name"].data;
    string type = eslot1["save"]["type"].data;
    
    print("Saving slot-1 ",type,".. ");
    
    if(type == "EEPROM") interface->saveRequest(Slot1EEPROM, file);
    if(type == "Flash")  interface->saveRequest(Slot1Flash, file);
    if(type == "FRAM")   interface->saveRequest(Slot1FRAM, file);
    
    print("\n");
  }
}

void Interface::save(unsigned id, const stream& memory) {
  if(id == Firmware) return system.saveFirmware(memory);
  if(id == Clock)    return system.saveRTC(memory);

  if(slot1.card && (id == Slot1EEPROM || id == Slot1Flash || id == Slot1FRAM)) {
    if(auto save = slot1.card->spi) {
      if(auto irport = dynamic_cast<IRPort*>(save))
        save = irport->slave;
      
      if(auto media = dynamic_cast<StaticMemory*>(save)) {
        memory.write(media->data, media->size);
        print("OK.");
      }
    }
  }
}



void Interface::unload() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  system.clock.freeze(tv.tv_sec, tv.tv_usec);
  system.running = false;
  
  save();
  delete slot1.unload();
}

void Interface::power() {
  system.power();
}

void Interface::run() {
  if(!system.running) {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    system.clock.thaw(tv.tv_sec, tv.tv_usec);
    system.running = true;
  }
  system.run();
}

serializer Interface::serialize() {
  return {};
}

bool Interface::unserialize(serializer &s) {
  return false;
}

void Interface::paletteUpdate() {
  for(unsigned color = 0; color < 01000000; color++) {
    uint16 r = uint6(color>> 0) * 010101/4;
    uint16 g = uint6(color>> 6) * 010101/4;
    uint16 b = uint6(color>>12) * 010101/4;
    
    palette[color] = interface->videoColor(color, r,g,b);
  }
}

void Interface::videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  static uint32_t pixels[256*384];
  
  for(unsigned y = 0; y < 384; y++) {
    const uint32 *src = &data[y*pitch/4];
    uint32 *dest = &pixels[y*256];
    
    for(unsigned x = 0; x < 256; x++)
      dest[x] = palette[src[x] & 0777777];
  }
  return bind->videoRefresh(pixels, 256*4, 256, 384);
}


Interface::Interface() {
  interface = this;
  
  information.name = "Nintendo DS";
  information.width = 256;
  information.height = 384;
  information.aspectRatio = 1.0;
  information.overscan = false;
  information.resettable = false;
  information.capability.states = false;
  information.capability.cheats = false;
  
  media.append({NintendoDS, "Nintendo DS", "nds"});
  //media.append({NintendoDS, "Nintendo DS", "nds", "Menu"});
  
  // Input devices and ports
  
  emptySlot    = Device{ID::Device::Empty, 1<<ID::Port::Slot1|1<<ID::Port::Slot2, "Empty"};
  
  // Slot 1 devices
  gameCard     = Device{ID::Device::GameCard, 1<<ID::Port::Slot1, "Game Card"};
  
  // Slot 2 devices
  gamePak      = Device{ID::Device::GamePak,      1<<ID::Port::Slot2, "Game Pak"};
  expansionPak = Device{ID::Device::ExpansionPak, 1<<ID::Port::Slot2, "Expansion Pak"};
  rumblePak    = Device{ID::Device::RumblePak,    1<<ID::Port::Slot2, "Rumble Pak"};
  guitarGrip   = Device{ID::Device::GuitarGrip,   1<<ID::Port::Slot2, "Guitar Grip"};
  piano        = Device{ID::Device::Piano,        1<<ID::Port::Slot2, "Piano"};
  paddle       = Device{ID::Device::Paddle,       1<<ID::Port::Slot2, "Paddle"};
  
    guitarGrip.input.append({ID::GuitarGrip::Green,  0, "Green"});
    guitarGrip.input.append({ID::GuitarGrip::Red,    0, "Red"});
    guitarGrip.input.append({ID::GuitarGrip::Yellow, 0, "Yellow"});
    guitarGrip.input.append({ID::GuitarGrip::Blue,   0, "Blue"});
    guitarGrip.order.append(0,1,2,3);
    
    piano.input.append({ID::Piano::C,  0, "C"});
    piano.input.append({ID::Piano::Cs, 0, "C#"});
    piano.input.append({ID::Piano::D,  0, "D"});
    piano.input.append({ID::Piano::Ds, 0, "D#"});
    piano.input.append({ID::Piano::E,  0, "E"});
    piano.input.append({ID::Piano::F,  0, "F"});
    piano.input.append({ID::Piano::Fs, 0, "F#"});
    piano.input.append({ID::Piano::G,  0, "G"});
    piano.input.append({ID::Piano::Gs, 0, "G#"});
    piano.input.append({ID::Piano::A,  0, "A"});
    piano.input.append({ID::Piano::As, 0, "A#"});
    piano.input.append({ID::Piano::B,  0, "B"});
    piano.input.append({ID::Piano::Ch, 0, "C - high"});
    piano.order.append(0,1,2,3,4,5,6,7,8,9,10,11,12);
    
    paddle.input.append({ID::Paddle::Rotation, 1, "Rotation"});
    paddle.order.append(0);

  // Internal inputs
  buttons  = Device{ID::Device::BuiltIn, 1<<ID::Port::Buttons, "Built-in"};
  sensors  = Device{ID::Device::BuiltIn, 1<<ID::Port::Sensors, "Built-in"};
  touchpad = Device{ID::Device::BuiltIn, 1<<ID::Port::Touchpad, "Built-in"};
  
    sensors.input.append({ID::Sensors::Lid,         0, "Lid closed"});
    sensors.input.append({ID::Sensors::Battery,     0, "Low battery"});
    sensors.input.append({ID::Sensors::Mains,       0, "Charging"});
    sensors.input.append({ID::Sensors::Headphones,  0, "Headphones"});  // ?
    sensors.input.append({ID::Sensors::Temperature, 2, "Temperature"});
    sensors.input.append({ID::Sensors::FIQ,         0, "FIQ - debug"});
    sensors.input.append({ID::Sensors::Reset,       0, "Reset - debug"});
    sensors.order.append(0,1,2,3,4,5,6);
    
    buttons.input.append({ID::Buttons::A, 0, "A"});
    buttons.input.append({ID::Buttons::B, 0, "B"});
    buttons.input.append({ID::Buttons::Select, 0, "Select"});
    buttons.input.append({ID::Buttons::Start, 0, "Start"});
    buttons.input.append({ID::Buttons::Right, 0, "Right"});
    buttons.input.append({ID::Buttons::Left, 0, "Left"});
    buttons.input.append({ID::Buttons::Up, 0, "Up"});
    buttons.input.append({ID::Buttons::Down, 0, "Down"});
    buttons.input.append({ID::Buttons::R, 0, "R"});
    buttons.input.append({ID::Buttons::L, 0, "L"});
    buttons.input.append({ID::Buttons::X, 0, "X"});
    buttons.input.append({ID::Buttons::Y, 0, "Y"});
    buttons.input.append({ID::Buttons::C, 0, "C - debug"}); // unconnected inputs
    buttons.input.append({ID::Buttons::D, 0, "D - debug"}); // <- present on debug units?
    buttons.input.append({ID::Buttons::Z, 0, "Z - debug"}); //
    buttons.input.append({ID::Buttons::W, 0, "W - debug"}); //
    buttons.order.append(6,7,5,4,2,3, 1,0,10,11,9,8, 12,13,14,15);
    
    touchpad.input.append({ID::Touchpad::X, 2, "X position"});
    touchpad.input.append({ID::Touchpad::Y, 2, "Y position"});
    touchpad.input.append({ID::Touchpad::Pressure, 2, "Pressure"});// (analog)"});
    //touchpad.input.append({ID::Touchpad::PressureD, 0, "Pressure (digital)"});
    touchpad.order.append(0,1,2);//,3);
  
  // Ports
  port.append({ID::Port::Buttons, "Buttons"});
  port.append({ID::Port::Sensors, "Sensors"});
  port.append({ID::Port::Touchpad, "Touchpad"});
  port.append({ID::Port::Slot1, "Slot 1"});
  port.append({ID::Port::Slot2, "Slot 2"});
  
  device.append(buttons);
  device.append(sensors);
  device.append(touchpad);
  device.append(emptySlot);
  device.append(gameCard);
  device.append(gamePak);
  device.append(expansionPak);
  device.append(rumblePak);
  device.append(guitarGrip);
  device.append(piano);
  device.append(paddle);
  
  for(auto &port : this->port)
    for(auto &device : this->device)
      if(device.portmask & 1<<port.id)
        port.device.append(device);
}

}
