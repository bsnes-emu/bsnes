void Ananke::copySuperFamicomSaves(const string &pathname) {
  if(!file::exists({pathname, "save.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".srm"})) {
      file::copy({information.path, nall::basename(information.name), ".srm"}, {pathname, "save.ram"});
    }
  }

  if(!file::exists({pathname, "rtc.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".rtc"})) {
      file::copy({information.path, nall::basename(information.name), ".rtc"}, {pathname, "rtc.ram"});
    }
  }
}

string Ananke::createSuperFamicomDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest) {
  string pathname = {
    userpath(), "Emulation/Super Famicom/",
    document["release/information/name"].text(),
    " (", document["release/information/region"].text(), ")",
    " (", document["release/information/revision"].text(), ")",
    ".sfc/"
  };
  directory::create(pathname);

  //strip "release" root node from database entry (since a single game manifest isn't part of a database)
  string markup = manifest;
  markup.replace("\n  ", "\n");
  markup.replace("information", "\ninformation");
  markup.ltrim<1>("release\n");

  file::write({pathname, "manifest.bml"}, markup);

  unsigned offset = 0;
  for(auto &node : document["release/information/configuration"]) {
    if(node.name != "rom") continue;
    string name = node["name"].text();
    unsigned size = node["size"].decimal();
    file::write({pathname, name}, buffer.data() + offset, size);
    offset += size;
  }

  copySuperFamicomSaves(pathname);
  return pathname;
}

string Ananke::createSuperFamicomHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    userpath(), "Emulation/Super Famicom/",
    nall::basename(information.name),
    " (!).sfc/"
  };
  directory::create(pathname);

  if((buffer.size() & 0x7fff) == 512) buffer.remove(0, 512);  //strip copier header, if present

  SuperFamicomCartridge info(buffer.data(), buffer.size());
  string markup = info.markup;
  markup.append("\ninformation\n  title: ", nall::basename(information.name), "\n");
  if(!information.manifest.empty()) markup = information.manifest;  //override with embedded beat manifest, if one exists
  information.manifest = markup;  //save for use with firmware routine below

  file::write({pathname, "manifest.bml"}, markup);

  if(!markup.position("spc7110")) {
    file::write({pathname, "program.rom"}, buffer.data(), info.rom_size);
  } else {
    file::write({pathname, "program.rom"}, buffer.data(), 0x100000);
    file::write({pathname, "data.rom"}, buffer.data() + 0x100000, info.rom_size - 0x100000);
  }

  createSuperFamicomHeuristicFirmware(buffer, pathname, info.firmware_appended);
  copySuperFamicomSaves(pathname);

  return pathname;
}

void Ananke::createSuperFamicomHeuristicFirmware(vector<uint8_t> &buffer, const string &pathname, bool firmware_appended) {
  auto copyFirmwareInternal = [&](const string &name, unsigned programSize, unsigned dataSize, unsigned bootSize) {
    //firmware appended directly onto .sfc file
    string basename = nall::basename(name);
    if(programSize) file::write({pathname, basename, ".program.rom"}, buffer.data() + buffer.size() - programSize - dataSize - bootSize, programSize);
    if(dataSize) file::write({pathname, basename, ".data.rom"}, buffer.data() + buffer.size() - dataSize - bootSize, dataSize);
    if(bootSize) file::write({pathname, basename, ".boot.rom"}, buffer.data() + buffer.size() - bootSize, bootSize);
  };

  auto copyFirmwareExternal = [&](const string &name, unsigned programSize, unsigned dataSize, unsigned bootSize) {
    //firmware stored in external file
    auto buffer = file::read({information.path, name});  //try and read from the containing directory
    if(buffer.size() == 0) buffer = extractFile(name);  //try and read from the containing archive, if one exists
    if(buffer.size() == 0) {
      MessageWindow::critical(Window::none(), {
        "Error: required firmware ", name, " not found. Game will not be playable!\n\n",
        "You must obtain this file, and place it in the same folder as this game."
      });
      return;
    }

    string basename = nall::basename(name);
    if(programSize) file::write({pathname, basename, ".program.rom"}, buffer.data(), programSize);
    if(dataSize) file::write({pathname, basename, ".data.rom"}, buffer.data() + programSize, dataSize);
    if(bootSize) file::write({pathname, basename, ".boot.rom"}, buffer.data() + programSize + dataSize, bootSize);
  };

  auto copyFirmware = [&](const string &name, unsigned programSize, unsigned dataSize, unsigned bootSize = 0) {
    if(firmware_appended == 1) copyFirmwareInternal(name, programSize, dataSize, bootSize);
    if(firmware_appended == 0) copyFirmwareExternal(name, programSize, dataSize, bootSize);
  };

  string markup = information.manifest;
  if(markup.position("dsp1.program.rom" )) copyFirmware("dsp1.rom",  0x001800, 0x000800);
  if(markup.position("dsp1b.program.rom")) copyFirmware("dsp1b.rom", 0x001800, 0x000800);
  if(markup.position("dsp2.program.rom" )) copyFirmware("dsp2.rom",  0x001800, 0x000800);
  if(markup.position("dsp3.program.rom" )) copyFirmware("dsp3.rom",  0x001800, 0x000800);
  if(markup.position("dsp4.program.rom" )) copyFirmware("dsp4.rom",  0x001800, 0x000800);
  if(markup.position("st010.program.rom")) copyFirmware("st010.rom", 0x00c000, 0x001000);
  if(markup.position("st011.program.rom")) copyFirmware("st011.rom", 0x00c000, 0x001000);
  if(markup.position("st018.program.rom")) copyFirmware("st018.rom", 0x020000, 0x008000);
  if(markup.position("cx4.data.rom"     )) copyFirmware("cx4.rom",   0x000000, 0x000c00);
  if(markup.position("sgb.boot.rom"     )) copyFirmware("sgb.rom",   0x000000, 0x000000, 0x000100);
}

string Ananke::openSuperFamicom(vector<uint8_t> &buffer) {
  string sha256 = nall::sha256(buffer.data(), buffer.size());

  string databaseText = string::read({configpath(), "ananke/database/Super Famicom.bml"}).strip();
  if(databaseText.empty()) databaseText = string{Database::SuperFamicom}.strip();
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);

    if(document["release/information/sha256"].text() == sha256) {
      return createSuperFamicomDatabase(buffer, document, item);
    }
  }

  return createSuperFamicomHeuristic(buffer);
}
