auto Cartridge::loadCartridge(Markup::Node node) -> void {
  information.title.cartridge = node["information/title"].text();
  auto board = node["board"];
  _region = board["region"].text() == "pal" ? Region::PAL : Region::NTSC;

  if(auto node = board["rom"]) loadROM(node);
  if(auto node = board["ram"]) loadRAM(node);
  if(auto node = board["icd2"]) loadICD2(node);
  if(auto node = board["mcc"]) loadMCC(node);
  if(auto node = board["bsmemory"]) loadBSMemoryPack(node);
  if(auto node = board.find("sufamiturbo")) if(node(0)) loadSufamiTurbo(node(0), 0);
  if(auto node = board.find("sufamiturbo")) if(node(1)) loadSufamiTurbo(node(1), 1);
  if(auto node = board["nss"]) loadNSS(node);
  if(auto node = board["event"]) loadEvent(node);
  if(auto node = board["sa1"]) loadSA1(node);
  if(auto node = board["superfx"]) loadSuperFX(node);
  if(auto node = board["armdsp"]) loadARMDSP(node);
  if(auto node = board["hitachidsp"]) loadHitachiDSP(node, node["information/board"].text().match("2DC*") ? 2 : 1);
  if(auto node = board["necdsp"]) loadNECDSP(node);
  if(auto node = board["epsonrtc"]) loadEpsonRTC(node);
  if(auto node = board["sharprtc"]) loadSharpRTC(node);
  if(auto node = board["spc7110"]) loadSPC7110(node);
  if(auto node = board["sdd1"]) loadSDD1(node);
  if(auto node = board["obc1"]) loadOBC1(node);
  if(auto node = board["msu1"]) loadMSU1(node);

  if(board["mcc"] || board["bsmemory"]) interface->loadRequest(ID::BSMemory, "BS Memory", "bs", false);
  if(board["sufamiturbo"]) interface->loadRequest(ID::SufamiTurboA, "Sufami Turbo", "st", false);
}

auto Cartridge::loadGameBoy(Markup::Node node) -> void {
}

auto Cartridge::loadBSMemory(Markup::Node node) -> void {
  information.title.bsMemory = node["information/title"].text();
  bsmemory.readonly = (node["board/rom/type"].text() == "mrom");

  loadMemory(bsmemory.memory, node["board/rom"], false);
}

auto Cartridge::loadSufamiTurboA(Markup::Node node) -> void {
  information.title.sufamiTurboA = node["information/title"].text();

  loadMemory(sufamiturboA.rom, node["rom"], ID::SufamiTurboA);
  loadMemory(sufamiturboA.ram, node["ram"], ID::SufamiTurboA);

  if(node["board/linkable"]) interface->loadRequest(ID::SufamiTurboB, "Sufami Turbo", "st", false);
}

auto Cartridge::loadSufamiTurboB(Markup::Node node) -> void {
  information.title.sufamiTurboB = node["information/title"].text();

  loadMemory(sufamiturboB.rom, node["rom"], ID::SufamiTurboB);
  loadMemory(sufamiturboB.ram, node["ram"], ID::SufamiTurboB);
}

//

auto Cartridge::loadROM(Markup::Node root) -> void {
  loadMemory(rom, root, false);
  for(auto node : root.find("map")) loadMap(node, rom);
}

auto Cartridge::loadRAM(Markup::Node root) -> void {
  loadMemory(ram, root, true);
  for(auto node : root.find("map")) loadMap(node, ram);
}

auto Cartridge::loadICD2(Markup::Node root) -> void {
  has.GameBoySlot = true;
  has.ICD2 = true;
  icd2.revision = max(1, root["revision"].natural());

  //Game Boy core loads data through ICD2 interface
  for(auto node : root.find("map")) loadMap(node, {&ICD2::read, &icd2}, {&ICD2::write, &icd2});
}

auto Cartridge::loadMCC(Markup::Node root) -> void {
  has.BSMemorySlot = true;
  has.MCC = true;

  loadMemory(mcc.rom, root["rom"], false);
  loadMemory(mcc.ram, root["ram"], true);

  for(auto node : root.find("map")) node.text() == "mcu"
  ? loadMap(node, {&MCC::mcuRead, &mcc}, {&MCC::mcuWrite, &mcc})
  : loadMap(node, {&MCC::read, &mcc}, {&MCC::write, &mcc});
  for(auto node : root["ram"].find("map")) loadMap(node, mcc.ram);
}

auto Cartridge::loadBSMemoryPack(Markup::Node root) -> void {
  has.BSMemorySlot = true;

  for(auto node : root.find("map")) {
    if(bsmemory.memory.size() == 0) continue;
    loadMap(node, bsmemory);
  }
}

auto Cartridge::loadSufamiTurbo(Markup::Node root, bool slot) -> void {
  has.SufamiTurboSlots = true;

  for(auto node : root["rom"].find("map")) {
    auto& cart = (slot == 0 ? sufamiturboA : sufamiturboB);
    if(cart.rom.size() == 0) continue;
    loadMap(node, cart.rom);
  }

  for(auto node : root["ram"].find("map")) {
    auto& cart = (slot == 0 ? sufamiturboA : sufamiturboB);
    if(cart.ram.size() == 0) continue;
    loadMap(node, cart.ram);
  }
}

auto Cartridge::loadNSS(Markup::Node root) -> void {
  has.NSSDIP = true;
  nss.dip = interface->dipSettings(root);

  for(auto node : root.find("map")) loadMap(node, {&NSS::read, &nss}, {&NSS::write, &nss});
}

auto Cartridge::loadEvent(Markup::Node root) -> void {
  auto roms = root.find("rom");
  if(roms.size() != 4) return;

  has.Event = true;

  for(uint n : range(4)) loadMemory(event.rom[n], roms[n], false);
  loadMemory(event.ram, root["ram"], true);

  event.board = Event::Board::CampusChallenge92;
  if(root.text() == "CC92") event.board = Event::Board::CampusChallenge92;
  if(root.text() == "PF94") event.board = Event::Board::Powerfest94;
  event.timer = root["timer"].natural();

  for(auto node : root.find("map")) node.text() == "mcu"
  ? loadMap(node, {&Event::mcuRead, &event}, {&Event::mcuWrite, &event})
  : loadMap(node, {&Event::read, &event}, {&Event::write, &event});
  for(auto node : root["ram"].find("map")) loadMap(node, event.ram);
}

auto Cartridge::loadSA1(Markup::Node root) -> void {
  has.SA1 = true;

  loadMemory(sa1.rom, root["rom"], false);
  loadMemory(sa1.bwram, root["bwram"], true);
  loadMemory(sa1.iram, root["iram"], true);

  for(auto node : root.find("map")) loadMap(node, {&SA1::readIO, &sa1}, {&SA1::writeIO, &sa1});
  for(auto node : root["rom"].find("map")) loadMap(node, {&SA1::mmcromRead, &sa1}, {&SA1::mmcromWrite, &sa1});
  for(auto node : root["bwram"].find("map")) loadMap(node, {&SA1::mmcbwramRead, &sa1}, {&SA1::mmcbwramWrite, &sa1});
  for(auto node : root["iram"].find("map")) loadMap(node, sa1.cpuiram);
}

auto Cartridge::loadSuperFX(Markup::Node root) -> void {
  has.SuperFX = true;

  loadMemory(superfx.rom, root["rom"], false);
  loadMemory(superfx.ram, root["ram"], true);

  for(auto node : root.find("map")) loadMap(node, {&SuperFX::readIO, &superfx}, {&SuperFX::writeIO, &superfx});
  for(auto node : root["rom"].find("map")) loadMap(node, superfx.cpurom);
  for(auto node : root["ram"].find("map")) loadMap(node, superfx.cpuram);
}

auto Cartridge::loadARMDSP(Markup::Node root) -> void {
  has.ARMDSP = true;

  if(auto fp = interface->open(ID::SuperFamicom, root["prom"]["name"].text(), File::Read, File::Required)) {
    for(auto n : range(128 * 1024)) armdsp.programROM[n] = fp->read();
  }
  if(auto fp = interface->open(ID::SuperFamicom, root["drom"]["name"].text(), File::Read, File::Required)) {
    for(auto n : range( 32 * 1024)) armdsp.dataROM[n] = fp->read();
  }
  if(auto fp = interface->open(ID::SuperFamicom, root["ram"]["name"].text(), File::Read)) {
    for(auto n : range( 16 * 1024)) armdsp.programRAM[n] = fp->read();
  }

  for(auto node : root.find("map")) loadMap(node, {&ArmDSP::read, &armdsp}, {&ArmDSP::write, &armdsp});
}

auto Cartridge::loadHitachiDSP(Markup::Node root, uint roms) -> void {
  has.HitachiDSP = true;

  hitachidsp.Frequency = root["frequency"].natural();
  if(hitachidsp.Frequency == 0) hitachidsp.frequency = 20000000;
  hitachidsp.Roms = roms;  //1 or 2

  loadMemory(hitachidsp.rom, root["rom"], false);
  loadMemory(hitachidsp.ram, root["ram"], true);

  for(auto& word : hitachidsp.dataROM) word = 0x000000;
  for(auto& word : hitachidsp.dataRAM) word = 0x00;

  if(auto fp = interface->open(ID::SuperFamicom, root["drom"]["name"].text(), File::Read, File::Required)) {
    for(auto n : range(1 * 1024)) hitachidsp.dataROM[n] = fp->readl(3);
  }
  if(auto fp = interface->open(ID::SuperFamicom, root["dram"]["name"].text(), File::Read)) {
    for(auto n : range(3 * 1024)) hitachidsp.dataRAM[n] = fp->readl(1);
  }

  for(auto node : root.find("map")) loadMap(node, {&HitachiDSP::dspRead, &hitachidsp}, {&HitachiDSP::dspWrite, &hitachidsp});
  for(auto node : root["rom"].find("map")) loadMap(node, {&HitachiDSP::romRead, &hitachidsp}, {&HitachiDSP::romWrite, &hitachidsp});
  for(auto node : root["ram"].find("map")) loadMap(node, {&HitachiDSP::ramRead, &hitachidsp}, {&HitachiDSP::ramWrite, &hitachidsp});
  for(auto node : root["dram"].find("map")) loadMap(node, {&HitachiDSP::dramRead, &hitachidsp}, {&HitachiDSP::dramWrite, &hitachidsp});
}

auto Cartridge::loadNECDSP(Markup::Node root) -> void {
  has.NECDSP = true;

  necdsp.frequency = root["frequency"].natural();
  if(necdsp.frequency == 0) necdsp.frequency = 8000000;
  necdsp.revision
  = root["model"].text() == "uPD7725"  ? NECDSP::Revision::uPD7725
  : root["model"].text() == "uPD96050" ? NECDSP::Revision::uPD96050
  : NECDSP::Revision::uPD7725;

  for(auto& word : necdsp.programROM) word = 0x000000;
  for(auto& word : necdsp.dataROM) word = 0x0000;
  for(auto& word : necdsp.dataRAM) word = 0x0000;

  uint size[3] = {0};
  if(necdsp.revision == NECDSP::Revision::uPD7725 ) memory::assign(size,  2048, 1024,  256);
  if(necdsp.revision == NECDSP::Revision::uPD96050) memory::assign(size, 16384, 2048, 2048);

  if(auto fp = interface->open(ID::SuperFamicom, root["prom"]["name"].text(), File::Read, File::Required)) {
    for(auto n : range(size[0])) necdsp.programROM[n] = fp->readl(3);
  }
  if(auto fp = interface->open(ID::SuperFamicom, root["drom"]["name"].text(), File::Read, File::Required)) {
    for(auto n : range(size[1])) necdsp.dataROM[n] = fp->readl(2);
  }
  if(auto fp = interface->open(ID::SuperFamicom, root["dram"]["name"].text(), File::Read)) {
    for(auto n : range(size[2])) necdsp.dataRAM[n] = fp->readl(2);
  }

  for(auto node : root.find("map")) loadMap(node, {&NECDSP::read, &necdsp}, {&NECDSP::write, &necdsp});
  for(auto node : root["dram"].find("map")) loadMap(node, {&NECDSP::readRAM, &necdsp}, {&NECDSP::writeRAM, &necdsp});
}

auto Cartridge::loadEpsonRTC(Markup::Node root) -> void {
  has.EpsonRTC = true;

  if(auto fp = interface->open(ID::SuperFamicom, root["ram"]["name"].text(), File::Read)) {
    uint8 data[16] = {0};
    for(auto& byte : data) fp->read();
    epsonrtc.load(data);
  }

  for(auto node : root.find("map")) loadMap(node, {&EpsonRTC::read, &epsonrtc}, {&EpsonRTC::write, &epsonrtc});
}

auto Cartridge::loadSharpRTC(Markup::Node root) -> void {
  has.SharpRTC = true;

  if(auto fp = interface->open(ID::SuperFamicom, root["ram"]["name"].text(), File::Read)) {
    uint8 data[16] = {0};
    for(auto& byte : data) fp->read();
    sharprtc.load(data);
  }

  for(auto node : root.find("map")) loadMap(node, {&SharpRTC::read, &sharprtc}, {&SharpRTC::write, &sharprtc});
}

auto Cartridge::loadSPC7110(Markup::Node root) -> void {
  has.SPC7110 = true;

  loadMemory(spc7110.prom, root["prom"], false);
  loadMemory(spc7110.drom, root["drom"], false);
  loadMemory(spc7110.ram, root["ram"], true);

  for(auto node : root.find("map")) node.text() == "mcu"
  ? loadMap(node, {&SPC7110::mcuromRead, &spc7110}, {&SPC7110::mcuromWrite, &spc7110})
  : loadMap(node, {&SPC7110::read, &spc7110}, {&SPC7110::write, &spc7110});
  for(auto node : root["ram"].find("map")) loadMap(node, {&SPC7110::mcuramRead, &spc7110}, {&SPC7110::mcuramWrite, &spc7110});
}

auto Cartridge::loadSDD1(Markup::Node root) -> void {
  has.SDD1 = true;

  loadMemory(sdd1.rom, root["rom"], false);
  loadMemory(sdd1.ram, root["ram"], true);

  for(auto node : root.find("map")) loadMap(node, {&SDD1::read, &sdd1}, {&SDD1::write, &sdd1});
  for(auto node : root["rom"].find("map")) loadMap(node, {&SDD1::mcuromRead, &sdd1}, {&SDD1::mcuromWrite, &sdd1});
  for(auto node : root["ram"].find("map")) loadMap(node, {&SDD1::mcuramRead, &sdd1}, {&SDD1::mcuramWrite, &sdd1});
}

auto Cartridge::loadOBC1(Markup::Node root) -> void {
  has.OBC1 = true;

  loadMemory(obc1.ram, root["ram"], true);

  for(auto node : root.find("map")) loadMap(node, {&OBC1::read, &obc1}, {&OBC1::write, &obc1});
}

auto Cartridge::loadMSU1(Markup::Node root) -> void {
  has.MSU1 = true;

  for(auto node : root.find("map")) loadMap(node, {&MSU1::read, &msu1}, {&MSU1::write, &msu1});
}

//

auto Cartridge::loadMemory(MappedRAM& ram, Markup::Node node, bool writable, uint id) -> void {
  auto name = node["name"].text();
  auto size = node["size"].natural();
  ram.allocate(size);
  if(auto fp = interface->open(id, name, File::Read, !writable)) {  //treat ROM as required; RAM as optional
    fp->read(ram.data(), ram.size());
  }
}

auto Cartridge::loadMap(Markup::Node map, SuperFamicom::Memory& memory) -> void {
  auto addr = map["address"].text();
  auto size = map["size"].natural();
  auto base = map["base"].natural();
  auto mask = map["mask"].natural();
  if(size == 0) size = memory.size();
  if(size == 0) return;
  bus.map({&SuperFamicom::Memory::read, &memory}, {&SuperFamicom::Memory::write, &memory}, addr, size, base, mask);
}

auto Cartridge::loadMap(
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
