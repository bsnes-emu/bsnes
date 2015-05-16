#ifdef CARTRIDGE_CPP

void Cartridge::parse_markup(const char* markup) {
  auto document = BML::unserialize(markup);
  information.title.cartridge = document["information/title"].text();

  auto cartridge = document["cartridge"];
  region = cartridge["region"].text() != "PAL" ? Region::NTSC : Region::PAL;

  mapping.reset();
  parse_markup_cartridge(cartridge);
  parse_markup_icd2(cartridge["icd2"]);
  parse_markup_bsx(cartridge["bsx"]);
  parse_markup_satellaview(cartridge["satellaview"]);
  parse_markup_sufamiturbo(cartridge["sufamiturbo[0]"], 0);
  parse_markup_sufamiturbo(cartridge["sufamiturbo[1]"], 1);
  parse_markup_nss(cartridge["nss"]);
  parse_markup_event(cartridge["event"]);
  parse_markup_sa1(cartridge["sa1"]);
  parse_markup_superfx(cartridge["superfx"]);
  parse_markup_armdsp(cartridge["armdsp"]);
  parse_markup_hitachidsp(cartridge["hitachidsp"], cartridge["board/type"].text().match("2DC*") ? 2 : 1);
  parse_markup_necdsp(cartridge["necdsp"]);
  parse_markup_epsonrtc(cartridge["epsonrtc"]);
  parse_markup_sharprtc(cartridge["sharprtc"]);
  parse_markup_spc7110(cartridge["spc7110"]);
  parse_markup_sdd1(cartridge["sdd1"]);
  parse_markup_obc1(cartridge["obc1"]);
  parse_markup_msu1(cartridge["msu1"]);
}

//

void Cartridge::parse_markup_map(Mapping& m, Markup::Node map) {
  m.addr = map["address"].text();
  m.size = map["size"].decimal();
  m.base = map["base"].decimal();
  m.mask = map["mask"].decimal();
}

void Cartridge::parse_markup_memory(MappedRAM& ram, Markup::Node node, unsigned id, bool writable) {
  string name = node["name"].text();
  unsigned size = node["size"].decimal();
  ram.map(allocate<uint8>(size, 0xff), size);
  if(name) {
    interface->loadRequest(id, name);
    if(writable) memory.append({id, name});
  }
}

//

void Cartridge::parse_markup_cartridge(Markup::Node root) {
  if(!root) return;

  parse_markup_memory(rom, root["rom"], ID::ROM, false);
  parse_markup_memory(ram, root["ram"], ID::RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      Mapping m(rom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m(ram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_icd2(Markup::Node root) {
  if(!root) return;
  has_gb_slot = true;
  icd2.revision = max(1, root["revision"].decimal());

  GameBoy::cartridge.load_empty(GameBoy::System::Revision::SuperGameBoy);
  interface->loadRequest(ID::SuperGameBoy, "Game Boy", "gb");

  string bootROMName = root["rom/name"].text();
  interface->loadRequest(ID::SuperGameBoyBootROM, bootROMName);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&ICD2::read, &icd2}, {&ICD2::write, &icd2});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_bsx(Markup::Node root) {
  if(!root) return;
  has_bs_cart = true;
  has_bs_slot = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs");

  parse_markup_memory(bsxcartridge.rom, root["rom"], ID::BsxROM, false);
  parse_markup_memory(bsxcartridge.ram, root["ram"], ID::BsxRAM, true);
  parse_markup_memory(bsxcartridge.psram, root["psram"], ID::BsxPSRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom"
    || node["id"].text() == "ram") {
      Mapping m({&BSXCartridge::mcu_read, &bsxcartridge}, {&BSXCartridge::mcu_write, &bsxcartridge});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "io") {
      Mapping m({&BSXCartridge::mmio_read, &bsxcartridge}, {&BSXCartridge::mmio_write, &bsxcartridge});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_satellaview(Markup::Node root) {
  if(!root) return;
  has_bs_slot = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs");

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      if(satellaviewcartridge.memory.size() == 0) continue;

      Mapping m(satellaviewcartridge);
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sufamiturbo(Markup::Node root, bool slot) {
  if(!root) return;
  has_st_slots = true;

  if(slot == 0) {
    //load required slot A (will request slot B if slot A cartridge is linkable)
    interface->loadRequest(ID::SufamiTurboSlotA, "Sufami Turbo - Slot A", "st");
  }

  for(auto node : root.find("map")) {
    SufamiTurboCartridge& cart = (slot == 0 ? sufamiturboA : sufamiturboB);

    if(node["id"].text() == "rom") {
      if(cart.rom.size() == 0) continue;

      Mapping m(cart.rom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = cart.rom.size();
      if(m.size) mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      if(cart.ram.size() == 0) continue;

      Mapping m(cart.ram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = cart.ram.size();
      if(m.size) mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_nss(Markup::Node root) {
  if(!root) return;
  has_nss_dip = true;
  nss.dip = interface->dipSettings(root);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&NSS::read, &nss}, {&NSS::write, &nss});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_event(Markup::Node root) {
  if(!root) return;
  has_event = true;

  for(auto node : root.find("rom")) {
    unsigned id = node["id"].decimal();
    if(id > 3) continue;
    parse_markup_memory(event.rom[id], node, ID::EventROM0 + id, false);
  }
  parse_markup_memory(event.ram, root["ram"], ID::EventRAM, true);

  event.board = Event::Board::CampusChallenge92;
  if(root["name"].text() == "Campus Challenge '92") event.board = Event::Board::CampusChallenge92;
  if(root["name"].text() == "Powerfest '94") event.board = Event::Board::Powerfest94;

  event.revision = root["revision"].text() == "B" ? 2 : 1;
  lstring part = root["timer"].text().split<1>(":");
  if(part.size() == 1) event.timer = decimal(part(0));
  if(part.size() == 2) event.timer = decimal(part(0)) * 60 + decimal(part(1));

  for(auto node : root.find("map")) {
    if(node["id"].text() == "rom") {
      Mapping m({&Event::rom_read, &event}, [](unsigned, uint8) {});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&Event::ram_read, &event}, {&Event::ram_write, &event});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "dr") {
      Mapping m([](unsigned) -> uint8 { return cpu.regs.mdr; }, {&Event::dr, &event});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "sr") {
      Mapping m({&Event::sr, &event}, [](unsigned, uint8) {});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sa1(Markup::Node root) {
  if(!root) return;
  has_sa1 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parse_markup_memory(sa1.rom, rom(0), ID::SA1ROM, false);
  parse_markup_memory(sa1.bwram, ram(0), ID::SA1BWRAM, true);
  parse_markup_memory(sa1.iram, ram(1), ID::SA1IRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SA1::mmio_read, &sa1}, {&SA1::mmio_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SA1::mmcrom_read, &sa1}, {&SA1::mmcrom_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "bwram") {
      Mapping m({&SA1::mmcbwram_read, &sa1}, {&SA1::mmcbwram_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "iram") {
      Mapping m(sa1.cpuiram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = sa1.cpuiram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_superfx(Markup::Node root) {
  if(!root) return;
  has_superfx = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parse_markup_memory(superfx.rom, rom(0), ID::SuperFXROM, false);
  parse_markup_memory(superfx.ram, ram(0), ID::SuperFXRAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SuperFX::mmio_read, &superfx}, {&SuperFX::mmio_write, &superfx});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m(superfx.cpurom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = superfx.rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m(superfx.cpuram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = superfx.ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_armdsp(Markup::Node root) {
  if(!root) return;
  has_armdsp = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  string programROMName = rom(0)["name"].text();
  string dataROMName = rom(1)["name"].text();
  string dataRAMName = ram(0)["name"].text();

  interface->loadRequest(ID::ArmDSPPROM, programROMName);
  interface->loadRequest(ID::ArmDSPDROM, dataROMName);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::ArmDSPRAM, dataRAMName);
    memory.append({ID::ArmDSPRAM, dataRAMName});
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&ArmDSP::mmio_read, &armdsp}, {&ArmDSP::mmio_write, &armdsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_hitachidsp(Markup::Node root, unsigned roms) {
  if(!root) return;
  has_hitachidsp = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parse_markup_memory(hitachidsp.rom, rom(0), ID::HitachiDSPROM, false);
  parse_markup_memory(hitachidsp.ram, ram(0), ID::HitachiDSPRAM, true);

  for(auto& word : hitachidsp.dataROM) word = 0x000000;
  for(auto& word : hitachidsp.dataRAM) word = 0x00;

  hitachidsp.Frequency = root["frequency"].decimal();
  if(hitachidsp.Frequency == 0) hitachidsp.frequency = 20000000;
  hitachidsp.Roms = roms;

  string dataROMName = rom(1)["name"].text();
  string dataRAMName = ram(1)["name"].text();

  interface->loadRequest(ID::HitachiDSPDROM, dataROMName);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::HitachiDSPDRAM, dataRAMName);
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&HitachiDSP::dsp_read, &hitachidsp}, {&HitachiDSP::dsp_write, &hitachidsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&HitachiDSP::rom_read, &hitachidsp}, {&HitachiDSP::rom_write, &hitachidsp});
      parse_markup_map(m, node);
      if(m.size == 0) m.size = hitachidsp.rom.size();
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&HitachiDSP::ram_read, &hitachidsp}, {&HitachiDSP::ram_write, &hitachidsp});
      parse_markup_map(m, node);
      if(m.size == 0) m.size = hitachidsp.ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_necdsp(Markup::Node root) {
  if(!root) return;
  has_necdsp = true;

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
    interface->loadRequest(ID::Nec7725DSPPROM, programROMName);
    interface->loadRequest(ID::Nec7725DSPDROM, dataROMName);
    if(dataRAMName.empty() == false) {
      interface->loadRequest(ID::Nec7725DSPRAM, dataRAMName);
      memory.append({ID::Nec7725DSPRAM, dataRAMName});
    }
  }

  if(necdsp.revision == NECDSP::Revision::uPD96050) {
    interface->loadRequest(ID::Nec96050DSPPROM, programROMName);
    interface->loadRequest(ID::Nec96050DSPDROM, dataROMName);
    if(dataRAMName.empty() == false) {
      interface->loadRequest(ID::Nec96050DSPRAM, dataRAMName);
      memory.append({ID::Nec96050DSPRAM, dataRAMName});
    }
  }

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&NECDSP::read, &necdsp}, {&NECDSP::write, &necdsp});
      parse_markup_map(m, node);
      mapping.append(m);
      necdsp.Select = node["select"].decimal();
    }

    if(node["id"].text() == "ram") {
      Mapping m({&NECDSP::ram_read, &necdsp}, {&NECDSP::ram_write, &necdsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_epsonrtc(Markup::Node root) {
  if(!root) return;
  has_epsonrtc = true;

  string name = root["ram/name"].text();
  interface->loadRequest(ID::EpsonRTC, name);
  memory.append({ID::EpsonRTC, name});

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&EpsonRTC::read, &epsonrtc}, {&EpsonRTC::write, &epsonrtc});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sharprtc(Markup::Node root) {
  if(!root) return;
  has_sharprtc = true;

  string name = root["ram/name"].text();
  interface->loadRequest(ID::SharpRTC, name);
  memory.append({ID::SharpRTC, name});

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SharpRTC::read, &sharprtc}, {&SharpRTC::write, &sharprtc});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_spc7110(Markup::Node root) {
  if(!root) return;
  has_spc7110 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parse_markup_memory(spc7110.prom, rom(0), ID::SPC7110PROM, false);
  parse_markup_memory(spc7110.drom, rom(1), ID::SPC7110DROM, false);
  parse_markup_memory(spc7110.ram, ram(0), ID::SPC7110RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SPC7110::read, &spc7110}, {&SPC7110::write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SPC7110::mcurom_read, &spc7110}, {&SPC7110::mcurom_write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&SPC7110::mcuram_read, &spc7110}, {&SPC7110::mcuram_write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sdd1(Markup::Node root) {
  if(!root) return;
  has_sdd1 = true;

  auto rom = root.find("rom");
  auto ram = root.find("ram");

  parse_markup_memory(sdd1.rom, rom(0), ID::SDD1ROM, false);
  parse_markup_memory(sdd1.ram, ram(0), ID::SDD1RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&SDD1::read, &sdd1}, {&SDD1::write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "rom") {
      Mapping m({&SDD1::mcurom_read, &sdd1}, {&SDD1::mcurom_write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].text() == "ram") {
      Mapping m({&SDD1::mcuram_read, &sdd1}, {&SDD1::mcuram_write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_obc1(Markup::Node root) {
  if(!root) return;
  has_obc1 = true;

  parse_markup_memory(obc1.ram, root["ram"], ID::OBC1RAM, true);

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&OBC1::read, &obc1}, {&OBC1::write, &obc1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_msu1(Markup::Node root) {
  if(!root) return;
  has_msu1 = true;

  for(auto node : root.find("map")) {
    if(node["id"].text() == "io") {
      Mapping m({&MSU1::mmio_read, &msu1}, {&MSU1::mmio_write, &msu1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

Cartridge::Mapping::Mapping() {
  size = base = mask = 0;
}

Cartridge::Mapping::Mapping(SuperFamicom::Memory& memory) {
  reader = {&SuperFamicom::Memory::read,  &memory};
  writer = {&SuperFamicom::Memory::write, &memory};
  size = base = mask = 0;
}

Cartridge::Mapping::Mapping(const function<uint8 (unsigned)>& reader, const function<void (unsigned, uint8)>& writer) {
  this->reader = reader;
  this->writer = writer;
  size = base = mask = 0;
}

#endif
