auto Cartridge::parseMarkup(const string& markup) -> void {
  auto document = BML::unserialize(markup);
  auto information = document["information"];
  auto board = document["board"];

  this->information.title.cartridge = information["title"].text();
  _region = board["region"].text() == "pal" ? Region::PAL : Region::NTSC;

  if(auto node = board["rom"]) parseMarkupROM(node);
  if(auto node = board["ram"]) parseMarkupRAM(node);
  if(auto node = board["icd2"]) parseMarkupICD2(node);
  if(auto node = board["mcc"]) parseMarkupMCC(node);
  if(auto node = board["bsmemory"]) parseMarkupBSMemory(node);
  if(auto node = board.find("sufamiturbo")) if(node(0)) parseMarkupSufamiTurbo(node(0), 0);
  if(auto node = board.find("sufamiturbo")) if(node(1)) parseMarkupSufamiTurbo(node(1), 1);
  if(auto node = board["nss"]) parseMarkupNSS(node);
  if(auto node = board["event"]) parseMarkupEvent(node);
  if(auto node = board["sa1"]) parseMarkupSA1(node);
  if(auto node = board["superfx"]) parseMarkupSuperFX(node);
  if(auto node = board["armdsp"]) parseMarkupARMDSP(node);
  if(auto node = board["hitachidsp"]) parseMarkupHitachiDSP(node, document["information/board"].text().match("2DC*") ? 2 : 1);
  if(auto node = board["necdsp"]) parseMarkupNECDSP(node);
  if(auto node = board["epsonrtc"]) parseMarkupEpsonRTC(node);
  if(auto node = board["sharprtc"]) parseMarkupSharpRTC(node);
  if(auto node = board["spc7110"]) parseMarkupSPC7110(node);
  if(auto node = board["sdd1"]) parseMarkupSDD1(node);
  if(auto node = board["obc1"]) parseMarkupOBC1(node);
  if(auto node = board["msu1"]) parseMarkupMSU1(node);
}

auto Cartridge::parseMarkupMap(Markup::Node map, SuperFamicom::Memory& memory) -> void {
  auto addr = map["address"].text();
  auto size = map["size"].natural();
  auto base = map["base"].natural();
  auto mask = map["mask"].natural();
  if(size == 0) size = memory.size();
  if(size == 0) return;
  bus.map({&SuperFamicom::Memory::read, &memory}, {&SuperFamicom::Memory::write, &memory}, addr, size, base, mask);
}

auto Cartridge::parseMarkupMap(
  Markup::Node map,
  const function<uint8 (uint24, uint8)>& reader,
  const function<void (uint24, uint8)>& writer
) -> void {
  auto addr = map["address"].text();
  auto size = map["size"].natural();
  auto base = map["base"].natural();
  auto mask = map["mask"].natural();
  bus.map(reader, writer, addr, size, base, mask);
}

auto Cartridge::parseMarkupMemory(MappedRAM& ram, Markup::Node node, uint id, bool writable) -> void {
  string name = node["name"].text();
  uint size = node["size"].natural();
  bool save = !(bool)node["volatile"];
  ram.map(allocate<uint8>(size, 0xff), size);
  if(name) {
    interface->loadRequest(id, name, !writable);  //treat ROM as required; RAM as optional
    if(writable && save) memory.append({id, name});
  }
}

auto Cartridge::parseMarkupROM(Markup::Node root) -> void {
  parseMarkupMemory(rom, root, ID::ROM, false);
  for(auto node : root.find("map")) {
    parseMarkupMap(node, rom);
  }
}

auto Cartridge::parseMarkupRAM(Markup::Node root) -> void {
  parseMarkupMemory(ram, root, ID::RAM, true);
  for(auto node : root.find("map")) {
    parseMarkupMap(node, ram);
  }
}

auto Cartridge::parseMarkupICD2(Markup::Node root) -> void {
  hasGameBoySlot = true;
  hasICD2 = true;
  icd2.revision = max(1, root["revision"].natural());

  //Game Boy core loads data through ICD2 interface

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&ICD2::read, &icd2}, {&ICD2::write, &icd2});
  }
}

auto Cartridge::parseMarkupMCC(Markup::Node root) -> void {
  hasBSMemorySlot = true;
  hasMCC = true;

  interface->loadRequest(ID::BSMemory, "BS Memory", "bs", false);

  parseMarkupMemory(mcc.rom, root["rom"], ID::MCCROM, false);
  parseMarkupMemory(mcc.ram, root["ram"], ID::MCCRAM, true);

  for(auto node : root.find("map")) {
    if(node.text() == "mcu") {
      parseMarkupMap(node, {&MCC::mcuRead, &mcc}, {&MCC::mcuWrite, &mcc});
    } else {
      parseMarkupMap(node, {&MCC::read, &mcc}, {&MCC::write, &mcc});
    }
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, mcc.ram);
  }
}

auto Cartridge::parseMarkupBSMemory(Markup::Node root) -> void {
  hasBSMemorySlot = true;

  interface->loadRequest(ID::BSMemory, "BS Memory", "bs", false);

  for(auto node : root.find("map")) {
    if(bsmemory.memory.size() == 0) continue;
    parseMarkupMap(node, bsmemory);
  }
}

auto Cartridge::parseMarkupSufamiTurbo(Markup::Node root, bool slot) -> void {
  hasSufamiTurboSlots = true;

  if(slot == 0) {
    //load required slot A (will request slot B if slot A cartridge is linkable)
    interface->loadRequest(ID::SufamiTurboSlotA, "Sufami Turbo", "st", false);
  }

  for(auto node : root["rom"].find("map")) {
    SufamiTurboCartridge& cart = (slot == 0 ? sufamiturboA : sufamiturboB);
    if(cart.rom.size() == 0) continue;
    parseMarkupMap(node, cart.rom);
  }

  for(auto node : root["ram"].find("map")) {
    SufamiTurboCartridge& cart = (slot == 0 ? sufamiturboA : sufamiturboB);
    if(cart.ram.size() == 0) continue;
    parseMarkupMap(node, cart.ram);
  }
}

auto Cartridge::parseMarkupNSS(Markup::Node root) -> void {
  hasNSSDIP = true;
  nss.dip = interface->dipSettings(root);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&NSS::read, &nss}, {&NSS::write, &nss});
  }
}

auto Cartridge::parseMarkupEvent(Markup::Node root) -> void {
  auto roms = root.find("rom");
  if(roms.size() != 4) return;

  hasEvent = true;

  for(uint n : range(4)) {
    parseMarkupMemory(event.rom[n], roms[n], ID::EventROM0 + n, false);
  }
  parseMarkupMemory(event.ram, root["ram"], ID::EventRAM, true);

  event.board = Event::Board::CampusChallenge92;
  if(root.text() == "CC92") event.board = Event::Board::CampusChallenge92;
  if(root.text() == "PF94") event.board = Event::Board::Powerfest94;
  event.timer = root["timer"].natural();

  for(auto node : root.find("map")) {
    if(node.text() == "mcu") {
      parseMarkupMap(node, {&Event::mcuRead, &event}, {&Event::mcuWrite, &event});
    } else {
      parseMarkupMap(node, {&Event::read, &event}, {&Event::write, &event});
    }
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, event.ram);
  }
}

auto Cartridge::parseMarkupSA1(Markup::Node root) -> void {
  hasSA1 = true;

  parseMarkupMemory(sa1.rom, root["rom"], ID::SA1ROM, false);
  parseMarkupMemory(sa1.bwram, root["bwram"], ID::SA1BWRAM, true);
  parseMarkupMemory(sa1.iram, root["iram"], ID::SA1IRAM, true);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&SA1::readIO, &sa1}, {&SA1::writeIO, &sa1});
  }

  for(auto node : root["rom"].find("map")) {
    parseMarkupMap(node, {&SA1::mmcrom_read, &sa1}, {&SA1::mmcrom_write, &sa1});
  }

  for(auto node : root["bwram"].find("map")) {
    parseMarkupMap(node, {&SA1::mmcbwram_read, &sa1}, {&SA1::mmcbwram_write, &sa1});
  }

  for(auto node : root["iram"].find("map")) {
    parseMarkupMap(node, sa1.cpuiram);
  }
}

auto Cartridge::parseMarkupSuperFX(Markup::Node root) -> void {
  hasSuperFX = true;

  parseMarkupMemory(superfx.rom, root["rom"], ID::SuperFXROM, false);
  parseMarkupMemory(superfx.ram, root["ram"], ID::SuperFXRAM, true);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&SuperFX::readIO, &superfx}, {&SuperFX::writeIO, &superfx});
  }

  for(auto node : root["rom"].find("map")) {
    parseMarkupMap(node, superfx.cpurom);
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, superfx.cpuram);
  }
}

auto Cartridge::parseMarkupARMDSP(Markup::Node root) -> void {
  hasARMDSP = true;

  interface->loadRequest(ID::ArmDSPPROM, root["prom"]["name"].text(), true);
  interface->loadRequest(ID::ArmDSPDROM, root["drom"]["name"].text(), true);
  interface->loadRequest(ID::ArmDSPRAM, root["dram"]["name"].text(), false);
  memory.append({ID::ArmDSPRAM, root["dram"]["name"].text()});

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&ArmDSP::read, &armdsp}, {&ArmDSP::write, &armdsp});
  }
}

auto Cartridge::parseMarkupHitachiDSP(Markup::Node root, uint roms) -> void {
  hasHitachiDSP = true;

  hitachidsp.Frequency = root["frequency"].natural();
  if(hitachidsp.Frequency == 0) hitachidsp.frequency = 20000000;
  hitachidsp.Roms = roms;  //1 or 2

  for(auto& word : hitachidsp.dataROM) word = 0x000000;
  for(auto& word : hitachidsp.dataRAM) word = 0x00;

  interface->loadRequest(ID::HitachiDSPDROM, root["drom"]["name"].text(), true);
  interface->loadRequest(ID::HitachiDSPDRAM, root["dram"]["name"].text(), false);

  parseMarkupMemory(hitachidsp.rom, root["rom"], ID::HitachiDSPROM, false);
  parseMarkupMemory(hitachidsp.ram, root["ram"], ID::HitachiDSPRAM, true);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&HitachiDSP::dspRead, &hitachidsp}, {&HitachiDSP::dspWrite, &hitachidsp});
  }

  for(auto node : root["rom"].find("map")) {
    parseMarkupMap(node, {&HitachiDSP::romRead, &hitachidsp}, {&HitachiDSP::romWrite, &hitachidsp});
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, {&HitachiDSP::ramRead, &hitachidsp}, {&HitachiDSP::ramWrite, &hitachidsp});
  }

  for(auto node : root["dram"].find("map")) {
    parseMarkupMap(node, {&HitachiDSP::dramRead, &hitachidsp}, {&HitachiDSP::dramWrite, &hitachidsp});
  }
}

auto Cartridge::parseMarkupNECDSP(Markup::Node root) -> void {
  hasNECDSP = true;

  necdsp.frequency = root["frequency"].natural();
  if(necdsp.frequency == 0) necdsp.frequency = 8000000;
  necdsp.revision
  = root["model"].text() == "uPD7725"  ? NECDSP::Revision::uPD7725
  : root["model"].text() == "uPD96050" ? NECDSP::Revision::uPD96050
  : NECDSP::Revision::uPD7725;

  for(auto& word : necdsp.programROM) word = 0x000000;
  for(auto& word : necdsp.dataROM) word = 0x0000;
  for(auto& word : necdsp.dataRAM) word = 0x0000;

  if(necdsp.revision == NECDSP::Revision::uPD7725) {
    interface->loadRequest(ID::Nec7725DSPPROM, root["prom"]["name"].text(), true);
    interface->loadRequest(ID::Nec7725DSPDROM, root["drom"]["name"].text(), true);
    interface->loadRequest(ID::Nec7725DSPRAM, root["dram"]["name"].text(), false);
    memory.append({ID::Nec7725DSPRAM, root["dram"]["name"].text()});
  }

  if(necdsp.revision == NECDSP::Revision::uPD96050) {
    interface->loadRequest(ID::Nec96050DSPPROM, root["prom"]["name"].text(), true);
    interface->loadRequest(ID::Nec96050DSPDROM, root["drom"]["name"].text(), true);
    interface->loadRequest(ID::Nec96050DSPRAM, root["dram"]["name"].text(), false);
    memory.append({ID::Nec96050DSPRAM, root["dram"]["name"].text()});
  }

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&NECDSP::read, &necdsp}, {&NECDSP::write, &necdsp});
  }

  for(auto node : root["dram"].find("map")) {
    parseMarkupMap(node, {&NECDSP::readRAM, &necdsp}, {&NECDSP::writeRAM, &necdsp});
  }
}

auto Cartridge::parseMarkupEpsonRTC(Markup::Node root) -> void {
  hasEpsonRTC = true;

  interface->loadRequest(ID::EpsonRTC, root["ram"]["name"].text(), false);
  memory.append({ID::EpsonRTC, root["ram"]["name"].text()});

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&EpsonRTC::read, &epsonrtc}, {&EpsonRTC::write, &epsonrtc});
  }
}

auto Cartridge::parseMarkupSharpRTC(Markup::Node root) -> void {
  hasSharpRTC = true;

  interface->loadRequest(ID::SharpRTC, root["ram"]["name"].text(), false);
  memory.append({ID::SharpRTC, root["ram"]["name"].text()});

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&SharpRTC::read, &sharprtc}, {&SharpRTC::write, &sharprtc});
  }
}

auto Cartridge::parseMarkupSPC7110(Markup::Node root) -> void {
  hasSPC7110 = true;

  parseMarkupMemory(spc7110.prom, root["prom"], ID::SPC7110PROM, false);
  parseMarkupMemory(spc7110.drom, root["drom"], ID::SPC7110DROM, false);
  parseMarkupMemory(spc7110.ram, root["ram"], ID::SPC7110RAM, true);

  for(auto node : root.find("map")) {
    if(node.text() == "mcu") {
      parseMarkupMap(node, {&SPC7110::mcurom_read, &spc7110}, {&SPC7110::mcurom_write, &spc7110});
    } else {
      parseMarkupMap(node, {&SPC7110::read, &spc7110}, {&SPC7110::write, &spc7110});
    }
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, {&SPC7110::mcuram_read, &spc7110}, {&SPC7110::mcuram_write, &spc7110});
  }
}

auto Cartridge::parseMarkupSDD1(Markup::Node root) -> void {
  hasSDD1 = true;

  parseMarkupMemory(sdd1.rom, root["rom"], ID::SDD1ROM, false);
  parseMarkupMemory(sdd1.ram, root["ram"], ID::SDD1RAM, true);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&SDD1::read, &sdd1}, {&SDD1::write, &sdd1});
  }

  for(auto node : root["rom"].find("map")) {
    parseMarkupMap(node, {&SDD1::mcuromRead, &sdd1}, {&SDD1::mcuromWrite, &sdd1});
  }

  for(auto node : root["ram"].find("map")) {
    parseMarkupMap(node, {&SDD1::mcuramRead, &sdd1}, {&SDD1::mcuramWrite, &sdd1});
  }
}

auto Cartridge::parseMarkupOBC1(Markup::Node root) -> void {
  hasOBC1 = true;

  parseMarkupMemory(obc1.ram, root["ram"], ID::OBC1RAM, true);

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&OBC1::read, &obc1}, {&OBC1::write, &obc1});
  }
}

auto Cartridge::parseMarkupMSU1(Markup::Node root) -> void {
  hasMSU1 = true;

  for(auto node : root.find("map")) {
    parseMarkupMap(node, {&MSU1::read, &msu1}, {&MSU1::write, &msu1});
  }
}
