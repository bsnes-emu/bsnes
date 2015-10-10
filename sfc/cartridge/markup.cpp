#ifdef CARTRIDGE_CPP

Cartridge::Mapping::Mapping(SuperFamicom::Memory& memory) {
  this->reader = {&SuperFamicom::Memory::read,  &memory};
  this->writer = {&SuperFamicom::Memory::write, &memory};
}

Cartridge::Mapping::Mapping(const function<uint8 (unsigned)>& reader, const function<void (unsigned, uint8)>& writer) {
  this->reader = reader;
  this->writer = writer;
}

auto Cartridge::parseMarkup(const string& markup) -> void {
  auto document = BML::unserialize(markup);
  information.title.cartridge = document["information/title"].text();

  auto cartridge = document["cartridge"];
  _region = cartridge["region"].text() != "PAL" ? Region::NTSC : Region::PAL;

  mapping.reset();
  if(auto node = cartridge) parseMarkupCartridge(node);
  if(auto node = cartridge["icd2"]) parseMarkupICD2(node);
  if(auto node = cartridge["mcc"]) parseMarkupMCC(node);
  if(auto node = cartridge["satellaview"]) parseMarkupSatellaview(node);
  if(auto node = cartridge.find("sufamiturbo")) if(node(0)) parseMarkupSufamiTurbo(node(0), 0);
  if(auto node = cartridge.find("sufamiturbo")) if(node(1)) parseMarkupSufamiTurbo(node(1), 1);
  if(auto node = cartridge["nss"]) parseMarkupNSS(node);
  if(auto node = cartridge["event"]) parseMarkupEvent(node);
  if(auto node = cartridge["sa1"]) parseMarkupSA1(node);
  if(auto node = cartridge["superfx"]) parseMarkupSuperFX(node);
  if(auto node = cartridge["armdsp"]) parseMarkupARMDSP(node);
  if(auto node = cartridge["hitachidsp"]) parseMarkupHitachiDSP(node, cartridge["board/type"].text().match("2DC*") ? 2 : 1);
  if(auto node = cartridge["necdsp"]) parseMarkupNECDSP(node);
  if(auto node = cartridge["epsonrtc"]) parseMarkupEpsonRTC(node);
  if(auto node = cartridge["sharprtc"]) parseMarkupSharpRTC(node);
  if(auto node = cartridge["spc7110"]) parseMarkupSPC7110(node);
  if(auto node = cartridge["sdd1"]) parseMarkupSDD1(node);
  if(auto node = cartridge["obc1"]) parseMarkupOBC1(node);
  if(auto node = cartridge["msu1"]) parseMarkupMSU1(node);
}

auto Cartridge::parseMarkupMap(Mapping& m, Markup::Node map) -> void {
  m.addr = map["address"].text();
  m.size = map["size"].decimal();
  m.base = map["base"].decimal();
  m.mask = map["mask"].decimal();
}

auto Cartridge::parseMarkupMemory(MappedRAM& ram, Markup::Node node, unsigned id, bool writable) -> void {
  string name = node["name"].text();
  unsigned size = node["size"].decimal();
  ram.map(allocate<uint8>(size, 0xff), size);
  if(name) {
    interface->loadRequest(id, name, !writable);  //treat ROM as required; RAM as optional
    if(writable) memory.append({id, name});
  }
}

auto Cartridge::parseMarkupCartridge(Markup::Node root) -> void {
  parseMarkupMemory(rom, root["rom"], ID::ROM, false);
  parseMarkupMemory(ram, root["ram"], ID::RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      Mapping m(rom);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m(ram);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = ram.size();
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupICD2(Markup::Node root) -> void {
  hasSuperGameBoySlot = true;
  hasICD2 = true;
  icd2.revision = max(1, root["revision"].decimal());

  GameBoy::cartridge.load_empty(GameBoy::System::Revision::SuperGameBoy);
  interface->loadRequest(ID::SuperGameBoy, "Game Boy", "gb", false);

  string bootROMName = root["rom/name"].text();
  interface->loadRequest(ID::SuperGameBoyBootROM, bootROMName, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&ICD2::read, &icd2}, {&ICD2::write, &icd2});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupMCC(Markup::Node root) -> void {
  hasSatellaviewSlot = true;
  hasMCC = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs", false);

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(mcc.rom, rom(0), ID::MCCROM, false);
  parseMarkupMemory(mcc.ram, ram(0), ID::MCCRAM, true);
  parseMarkupMemory(mcc.psram, ram(1), ID::MCCPSRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom"
    || node["id"].text() == "ram") {
      Mapping m({&MCC::mcu_read, &mcc}, {&MCC::mcu_write, &mcc});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "io") {
      Mapping m({&MCC::mmio_read, &mcc}, {&MCC::mmio_write, &mcc});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSatellaview(Markup::Node root) -> void {
  hasSatellaviewSlot = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs", false);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      if(satellaviewcartridge.memory.size() == 0) continue;

      Mapping m(satellaviewcartridge);
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSufamiTurbo(Markup::Node root, bool slot) -> void {
  hasSufamiTurboSlots = true;

  if(slot == 0) {
    //load required slot A (will request slot B if slot A cartridge is linkable)
    interface->loadRequest(ID::SufamiTurboSlotA, "Sufami Turbo - Slot A", "st", false);
  }

  for(auto node : root.find("map")) {
    SufamiTurboCartridge& cart = (slot == 0 ? sufamiturboA : sufamiturboB);

    if(node["id"].text() == "rom") {
      if(cart.rom.size() == 0) continue;

      Mapping m(cart.rom);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = cart.rom.size();
      if(m.size) mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      if(cart.ram.size() == 0) continue;

      Mapping m(cart.ram);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = cart.ram.size();
      if(m.size) mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupNSS(Markup::Node root) -> void {
  hasNSSDIP = true;
  nss.dip = interface->dipSettings(root);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&NSS::read, &nss}, {&NSS::write, &nss});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupEvent(Markup::Node root) -> void {
  hasEvent = true;

  for(auto node : root.find("rom")) {
    unsigned id = node["id"].decimal();
    if(id > 3) continue;
    parseMarkupMemory(event.rom[id], node, ID::EventROM0 + id, false);
  }
  parseMarkupMemory(event.ram, root["ram"], ID::EventRAM, true);

  event.board = Event::Board::CampusChallenge92;
  if(root["name"].text() == "Campus Challenge '92") event.board = Event::Board::CampusChallenge92;
  if(root["name"].text() == "Powerfest '94") event.board = Event::Board::Powerfest94;

  event.revision = root["revision"].text() == "B" ? 2 : 1;
  lstring part = root["timer"].text().split(":", 1L);
  if(part.size() == 1) event.timer = decimal(part(0));
  if(part.size() == 2) event.timer = decimal(part(0)) * 60 + decimal(part(1));

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      Mapping m({&Event::rom_read, &event}, [](unsigned, uint8) {});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&Event::ram_read, &event}, {&Event::ram_write, &event});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "dr") {
      Mapping m([](unsigned) -> uint8 { return cpu.regs.mdr; }, {&Event::dr, &event});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "sr") {
      Mapping m({&Event::sr, &event}, [](unsigned, uint8) {});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSA1(Markup::Node root) -> void {
  hasSA1 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(sa1.rom, rom(0), ID::SA1ROM, false);
  parseMarkupMemory(sa1.bwram, ram(0), ID::SA1BWRAM, true);
  parseMarkupMemory(sa1.iram, ram(1), ID::SA1IRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SA1::mmio_read, &sa1}, {&SA1::mmio_write, &sa1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SA1::mmcrom_read, &sa1}, {&SA1::mmcrom_write, &sa1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "bwram") {
      Mapping m({&SA1::mmcbwram_read, &sa1}, {&SA1::mmcbwram_write, &sa1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "iram") {
      Mapping m(sa1.cpuiram);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = sa1.cpuiram.size();
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSuperFX(Markup::Node root) -> void {
  hasSuperFX = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(superfx.rom, rom(0), ID::SuperFXROM, false);
  parseMarkupMemory(superfx.ram, ram(0), ID::SuperFXRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SuperFX::mmio_read, &superfx}, {&SuperFX::mmio_write, &superfx});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m(superfx.cpurom);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = superfx.rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m(superfx.cpuram);
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = superfx.ram.size();
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupARMDSP(Markup::Node root) -> void {
  hasARMDSP = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  string programROMName = rom(0)["name"].text();
  string dataROMName = rom(1)["name"].text();
  string dataRAMName = ram(0)["name"].text();

  interface->loadRequest(ID::ArmDSPPROM, programROMName, true);
  interface->loadRequest(ID::ArmDSPDROM, dataROMName, true);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::ArmDSPRAM, dataRAMName, false);
    memory.append({ID::ArmDSPRAM, dataRAMName});
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&ArmDSP::mmio_read, &armdsp}, {&ArmDSP::mmio_write, &armdsp});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupHitachiDSP(Markup::Node root, unsigned roms) -> void {
  hasHitachiDSP = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(hitachidsp.rom, rom(0), ID::HitachiDSPROM, false);
  parseMarkupMemory(hitachidsp.ram, ram(0), ID::HitachiDSPRAM, true);

  for(auto& word : hitachidsp.dataROM) word = 0x000000;
  for(auto& word : hitachidsp.dataRAM) word = 0x00;

  hitachidsp.Frequency = root["frequency"].decimal();
  if(hitachidsp.Frequency == 0) hitachidsp.frequency = 20000000;
  hitachidsp.Roms = roms;

  string dataROMName = rom(1)["name"].text();
  string dataRAMName = ram(1)["name"].text();

  interface->loadRequest(ID::HitachiDSPDROM, dataROMName, true);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::HitachiDSPDRAM, dataRAMName, false);
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&HitachiDSP::dsp_read, &hitachidsp}, {&HitachiDSP::dsp_write, &hitachidsp});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&HitachiDSP::rom_read, &hitachidsp}, {&HitachiDSP::rom_write, &hitachidsp});
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = hitachidsp.rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&HitachiDSP::ram_read, &hitachidsp}, {&HitachiDSP::ram_write, &hitachidsp});
      parseMarkupMap(m, node);
      if(m.size == 0) m.size = hitachidsp.ram.size();
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupNECDSP(Markup::Node root) -> void {
  hasNECDSP = true;

  for(auto& word : necdsp.programROM) word = 0x000000;
  for(auto& word : necdsp.dataROM) word = 0x0000;
  for(auto& word : necdsp.dataRAM) word = 0x0000;

  necdsp.frequency = root["frequency"].decimal();
  if(necdsp.frequency == 0) necdsp.frequency = 8000000;
  necdsp.revision
  = root["model"].text() == "uPD7725"  ? NECDSP::Revision::uPD7725
  : root["model"].text() == "uPD96050" ? NECDSP::Revision::uPD96050
  : NECDSP::Revision::uPD7725;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  string programROMName = rom(0)["name"].text();
  string dataROMName = rom(1)["name"].text();
  string dataRAMName = ram(0)["name"].text();

  if(necdsp.revision == NECDSP::Revision::uPD7725) {
    interface->loadRequest(ID::Nec7725DSPPROM, programROMName, true);
    interface->loadRequest(ID::Nec7725DSPDROM, dataROMName, true);
    if(dataRAMName.empty() == false) {
      interface->loadRequest(ID::Nec7725DSPRAM, dataRAMName, false);
      memory.append({ID::Nec7725DSPRAM, dataRAMName});
    }
  }

  if(necdsp.revision == NECDSP::Revision::uPD96050) {
    interface->loadRequest(ID::Nec96050DSPPROM, programROMName, true);
    interface->loadRequest(ID::Nec96050DSPDROM, dataROMName, true);
    if(dataRAMName.empty() == false) {
      interface->loadRequest(ID::Nec96050DSPRAM, dataRAMName, false);
      memory.append({ID::Nec96050DSPRAM, dataRAMName});
    }
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&NECDSP::read, &necdsp}, {&NECDSP::write, &necdsp});
      parseMarkupMap(m, node);
      mapping.append(m);
      necdsp.Select = node["select"].decimal();
    }

    if(node["id"].text() == "ram") {
      Mapping m({&NECDSP::ram_read, &necdsp}, {&NECDSP::ram_write, &necdsp});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupEpsonRTC(Markup::Node root) -> void {
  hasEpsonRTC = true;

  string name = root["ram/name"].text();
  interface->loadRequest(ID::EpsonRTC, name, false);
  memory.append({ID::EpsonRTC, name});

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&EpsonRTC::read, &epsonrtc}, {&EpsonRTC::write, &epsonrtc});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSharpRTC(Markup::Node root) -> void {
  hasSharpRTC = true;

  string name = root["ram/name"].text();
  interface->loadRequest(ID::SharpRTC, name, false);
  memory.append({ID::SharpRTC, name});

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SharpRTC::read, &sharprtc}, {&SharpRTC::write, &sharprtc});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSPC7110(Markup::Node root) -> void {
  hasSPC7110 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(spc7110.prom, rom(0), ID::SPC7110PROM, false);
  parseMarkupMemory(spc7110.drom, rom(1), ID::SPC7110DROM, false);
  parseMarkupMemory(spc7110.ram, ram(0), ID::SPC7110RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SPC7110::read, &spc7110}, {&SPC7110::write, &spc7110});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SPC7110::mcurom_read, &spc7110}, {&SPC7110::mcurom_write, &spc7110});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&SPC7110::mcuram_read, &spc7110}, {&SPC7110::mcuram_write, &spc7110});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupSDD1(Markup::Node root) -> void {
  hasSDD1 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parseMarkupMemory(sdd1.rom, rom(0), ID::SDD1ROM, false);
  parseMarkupMemory(sdd1.ram, ram(0), ID::SDD1RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SDD1::read, &sdd1}, {&SDD1::write, &sdd1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SDD1::mcurom_read, &sdd1}, {&SDD1::mcurom_write, &sdd1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&SDD1::mcuram_read, &sdd1}, {&SDD1::mcuram_write, &sdd1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupOBC1(Markup::Node root) -> void {
  hasOBC1 = true;

  parseMarkupMemory(obc1.ram, root["ram"], ID::OBC1RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&OBC1::read, &obc1}, {&OBC1::write, &obc1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

auto Cartridge::parseMarkupMSU1(Markup::Node root) -> void {
  hasMSU1 = true;

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&MSU1::mmioRead, &msu1}, {&MSU1::mmioWrite, &msu1});
      parseMarkupMap(m, node);
      mapping.append(m);
    }
  }
}

#endif
