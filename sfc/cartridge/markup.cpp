#ifdef CARTRIDGE_CPP

void Cartridge::parse_markup(const char* markup) {
  auto document = Markup::Document(markup);
  information.title.cartridge = document["information/title"].text();

  auto cartridge = document["cartridge"];
  region = cartridge["region"].data != "PAL" ? Region::NTSC : Region::PAL;

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
  parse_markup_hitachidsp(cartridge["hitachidsp"], cartridge["board/type"].data.match("2DC*") ? 2 : 1);
  parse_markup_necdsp(cartridge["necdsp"]);
  parse_markup_epsonrtc(cartridge["epsonrtc"]);
  parse_markup_sharprtc(cartridge["sharprtc"]);
  parse_markup_spc7110(cartridge["spc7110"]);
  parse_markup_sdd1(cartridge["sdd1"]);
  parse_markup_obc1(cartridge["obc1"]);
  parse_markup_hsu1(cartridge["hsu1"]);
  parse_markup_msu1(cartridge["msu1"]);
}

//

void Cartridge::parse_markup_map(Mapping& m, Markup::Node map) {
  m.addr = map["address"].data;
  m.size = numeral(map["size"].data);
  m.base = numeral(map["base"].data);
  m.mask = numeral(map["mask"].data);
}

void Cartridge::parse_markup_memory(MappedRAM& ram, Markup::Node node, unsigned id, bool writable) {
  string name = node["name"].data;
  unsigned size = numeral(node["size"].data);
  ram.map(allocate<uint8>(size, 0xff), size);
  if(name.empty() == false) {
    interface->loadRequest(id, name);
    if(writable) memory.append({id, name});
  }
}

//

void Cartridge::parse_markup_cartridge(Markup::Node root) {
  if(root.exists() == false) return;

  parse_markup_memory(rom, root["rom"], ID::ROM, false);
  parse_markup_memory(ram, root["ram"], ID::RAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "rom") {
      Mapping m(rom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = rom.size();
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m(ram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_icd2(Markup::Node root) {
  if(root.exists() == false) return;
  has_gb_slot = true;
  icd2.revision = max(1, numeral(root["revision"].data));

  GameBoy::cartridge.load_empty(GameBoy::System::Revision::SuperGameBoy);
  interface->loadRequest(ID::SuperGameBoy, "Game Boy", "gb");

  string bootROMName = root["rom"]["name"].data;
  interface->loadRequest(ID::SuperGameBoyBootROM, bootROMName);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&ICD2::read, &icd2}, {&ICD2::write, &icd2});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_bsx(Markup::Node root) {
  if(root.exists() == false) return;
  has_bs_cart = true;
  has_bs_slot = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs");

  parse_markup_memory(bsxcartridge.rom, root["rom"], ID::BsxROM, false);
  parse_markup_memory(bsxcartridge.ram, root["ram"], ID::BsxRAM, true);
  parse_markup_memory(bsxcartridge.psram, root["psram"], ID::BsxPSRAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "rom"
    || node["id"].data == "ram") {
      Mapping m({&BSXCartridge::mcu_read, &bsxcartridge}, {&BSXCartridge::mcu_write, &bsxcartridge});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "io") {
      Mapping m({&BSXCartridge::mmio_read, &bsxcartridge}, {&BSXCartridge::mmio_write, &bsxcartridge});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_satellaview(Markup::Node root) {
  if(root.exists() == false) return;
  has_bs_slot = true;

  interface->loadRequest(ID::Satellaview, "BS-X Satellaview", "bs");

  for(auto& node : root.find("map")) {
    if(node["id"].data == "rom") {
      if(satellaviewcartridge.memory.size() == 0) continue;

      Mapping m(satellaviewcartridge);
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sufamiturbo(Markup::Node root, bool slot) {
  if(root.exists() == false) return;
  has_st_slots = true;

  if(slot == 0) {
    //load required slot A (will request slot B if slot A cartridge is linkable)
    interface->loadRequest(ID::SufamiTurboSlotA, "Sufami Turbo - Slot A", "st");
  }

  for(auto& node : root.find("map")) {
    SufamiTurboCartridge &cart = (slot == 0 ? sufamiturboA : sufamiturboB);

    if(node["id"].data == "rom") {
      if(cart.rom.size() == 0) continue;

      Mapping m(cart.rom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = cart.rom.size();
      if(m.size) mapping.append(m);
    }

    if(node["id"].data == "ram") {
      if(cart.ram.size() == 0) continue;

      Mapping m(cart.ram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = cart.ram.size();
      if(m.size) mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_nss(Markup::Node root) {
  if(root.exists() == false) return;
  has_nss_dip = true;
  nss.dip = interface->dipSettings(root);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&NSS::read, &nss}, {&NSS::write, &nss});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_event(Markup::Node root) {
  if(root.exists() == false) return;
  has_event = true;

  for(auto& node : root) {
    if(node.name != "rom") continue;
    unsigned id = numeral(node["id"].data);
    if(id > 3) continue;
    parse_markup_memory(event.rom[id], node, ID::EventROM0 + id, false);
  }
  parse_markup_memory(event.ram, root["ram"], ID::EventRAM, true);

  event.board = Event::Board::CampusChallenge92;
  if(root["name"].data == "Campus Challenge '92") event.board = Event::Board::CampusChallenge92;
  if(root["name"].data == "Powerfest '94") event.board = Event::Board::Powerfest94;

  event.revision = root["revision"].data == "B" ? 2 : 1;
  lstring part = root["timer"].data.split<1>(":");
  if(part.size() == 1) event.timer = decimal(part(0));
  if(part.size() == 2) event.timer = decimal(part(0)) * 60 + decimal(part(1));

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "rom") {
      Mapping m({&Event::rom_read, &event}, [](unsigned, uint8) {});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m({&Event::ram_read, &event}, {&Event::ram_write, &event});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "dr") {
      Mapping m([](unsigned) -> uint8 { return cpu.regs.mdr; }, {&Event::dr, &event});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "sr") {
      Mapping m({&Event::sr, &event}, [](unsigned, uint8) {});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sa1(Markup::Node root) {
  if(root.exists() == false) return;
  has_sa1 = true;

  parse_markup_memory(sa1.rom, root["rom"], ID::SA1ROM, false);
  parse_markup_memory(sa1.bwram, root["ram[0]"], ID::SA1BWRAM, true);
  parse_markup_memory(sa1.iram, root["ram[1]"], ID::SA1IRAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&SA1::mmio_read, &sa1}, {&SA1::mmio_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "rom") {
      Mapping m({&SA1::mmcrom_read, &sa1}, {&SA1::mmcrom_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "bwram") {
      Mapping m({&SA1::mmcbwram_read, &sa1}, {&SA1::mmcbwram_write, &sa1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "iram") {
      Mapping m(sa1.cpuiram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = sa1.cpuiram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_superfx(Markup::Node root) {
  if(root.exists() == false) return;
  has_superfx = true;

  parse_markup_memory(superfx.rom, root["rom"], ID::SuperFXROM, false);
  parse_markup_memory(superfx.ram, root["ram"], ID::SuperFXRAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&SuperFX::mmio_read, &superfx}, {&SuperFX::mmio_write, &superfx});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "rom") {
      Mapping m(superfx.cpurom);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = superfx.rom.size();
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m(superfx.cpuram);
      parse_markup_map(m, node);
      if(m.size == 0) m.size = superfx.ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_armdsp(Markup::Node root) {
  if(root.exists() == false) return;
  has_armdsp = true;

  string programROMName = root["rom[0]/name"].data;
  string dataROMName = root["rom[1]/name"].data;
  string dataRAMName = root["ram/name"].data;

  interface->loadRequest(ID::ArmDSPPROM, programROMName);
  interface->loadRequest(ID::ArmDSPDROM, dataROMName);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::ArmDSPRAM, dataRAMName);
    memory.append({ID::ArmDSPRAM, dataRAMName});
  }

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&ArmDSP::mmio_read, &armdsp}, {&ArmDSP::mmio_write, &armdsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_hitachidsp(Markup::Node root, unsigned roms) {
  if(root.exists() == false) return;
  has_hitachidsp = true;

  parse_markup_memory(hitachidsp.rom, root["rom[0]"], ID::HitachiDSPROM, false);
  parse_markup_memory(hitachidsp.ram, root["ram[0]"], ID::HitachiDSPRAM, true);

  for(auto& word : hitachidsp.dataROM) word = 0x000000;
  for(auto& word : hitachidsp.dataRAM) word = 0x00;

  hitachidsp.Frequency = numeral(root["frequency"].data);
  if(hitachidsp.Frequency == 0) hitachidsp.frequency = 20000000;
  hitachidsp.Roms = roms;

  string dataROMName = root["rom[1]/name"].data;
  string dataRAMName = root["ram[1]/name"].data;

  interface->loadRequest(ID::HitachiDSPDROM, dataROMName);
  if(dataRAMName.empty() == false) {
    interface->loadRequest(ID::HitachiDSPDRAM, dataRAMName);
  }

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&HitachiDSP::dsp_read, &hitachidsp}, {&HitachiDSP::dsp_write, &hitachidsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "rom") {
      Mapping m({&HitachiDSP::rom_read, &hitachidsp}, {&HitachiDSP::rom_write, &hitachidsp});
      parse_markup_map(m, node);
      if(m.size == 0) m.size = hitachidsp.rom.size();
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m({&HitachiDSP::ram_read, &hitachidsp}, {&HitachiDSP::ram_write, &hitachidsp});
      parse_markup_map(m, node);
      if(m.size == 0) m.size = hitachidsp.ram.size();
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_necdsp(Markup::Node root) {
  if(root.exists() == false) return;
  has_necdsp = true;

  for(auto& word : necdsp.programROM) word = 0x000000;
  for(auto& word : necdsp.dataROM) word = 0x0000;
  for(auto& word : necdsp.dataRAM) word = 0x0000;

  necdsp.frequency = numeral(root["frequency"].data);
  if(necdsp.frequency == 0) necdsp.frequency = 8000000;
  necdsp.revision
  = root["model"].data == "uPD7725"  ? NECDSP::Revision::uPD7725
  : root["model"].data == "uPD96050" ? NECDSP::Revision::uPD96050
  : NECDSP::Revision::uPD7725;

  string programROMName = root["rom[0]/name"].data;
  string dataROMName = root["rom[1]/name"].data;
  string dataRAMName = root["ram/name"].data;

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

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&NECDSP::read, &necdsp}, {&NECDSP::write, &necdsp});
      parse_markup_map(m, node);
      mapping.append(m);
      necdsp.Select = numeral(node["select"].data);
    }

    if(node["id"].data == "ram") {
      Mapping m({&NECDSP::ram_read, &necdsp}, {&NECDSP::ram_write, &necdsp});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_epsonrtc(Markup::Node root) {
  if(root.exists() == false) return;
  has_epsonrtc = true;

  string name = root["ram"]["name"].data;
  interface->loadRequest(ID::EpsonRTC, name);
  memory.append({ID::EpsonRTC, name});

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&EpsonRTC::read, &epsonrtc}, {&EpsonRTC::write, &epsonrtc});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sharprtc(Markup::Node root) {
  if(root.exists() == false) return;
  has_sharprtc = true;

  string name = root["ram"]["name"].data;
  interface->loadRequest(ID::SharpRTC, name);
  memory.append({ID::SharpRTC, name});

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&SharpRTC::read, &sharprtc}, {&SharpRTC::write, &sharprtc});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_spc7110(Markup::Node root) {
  if(root.exists() == false) return;
  has_spc7110 = true;

  parse_markup_memory(spc7110.prom, root["rom[0]"], ID::SPC7110PROM, false);
  parse_markup_memory(spc7110.drom, root["rom[1]"], ID::SPC7110DROM, false);
  parse_markup_memory(spc7110.ram, root["ram"], ID::SPC7110RAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&SPC7110::read, &spc7110}, {&SPC7110::write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "rom") {
      Mapping m({&SPC7110::mcurom_read, &spc7110}, {&SPC7110::mcurom_write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m({&SPC7110::mcuram_read, &spc7110}, {&SPC7110::mcuram_write, &spc7110});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_sdd1(Markup::Node root) {
  if(root.exists() == false) return;
  has_sdd1 = true;

  parse_markup_memory(sdd1.rom, root["rom"], ID::SDD1ROM, false);
  parse_markup_memory(sdd1.ram, root["ram"], ID::SDD1RAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&SDD1::read, &sdd1}, {&SDD1::write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "rom") {
      Mapping m({&SDD1::mcurom_read, &sdd1}, {&SDD1::mcurom_write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }

    if(node["id"].data == "ram") {
      Mapping m({&SDD1::mcuram_read, &sdd1}, {&SDD1::mcuram_write, &sdd1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_obc1(Markup::Node root) {
  if(root.exists() == false) return;
  has_obc1 = true;

  parse_markup_memory(obc1.ram, root["ram"], ID::OBC1RAM, true);

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&OBC1::read, &obc1}, {&OBC1::write, &obc1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_hsu1(Markup::Node root) {
  if(root.exists() == false) return;
  has_hsu1 = true;

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
      Mapping m({&HSU1::read, &hsu1}, {&HSU1::write, &hsu1});
      parse_markup_map(m, node);
      mapping.append(m);
    }
  }
}

void Cartridge::parse_markup_msu1(Markup::Node root) {
  if(root.exists() == false) return;
  has_msu1 = true;

  for(auto& node : root) {
    if(node.name != "map") continue;

    if(node["id"].data == "io") {
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
