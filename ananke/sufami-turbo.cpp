void Ananke::copySufamiTurboSaves(const string &pathname) {
  if(!file::exists({pathname, "save.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".srm"})) {
      file::copy({information.path, nall::basename(information.name), ".srm"}, {pathname, "save.ram"});
    }
  }
}

string Ananke::createSufamiTurboDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest) {
  string pathname = {
    userpath(), "Emulation/Sufami Turbo/",
    document["release/information/name"].text(),
    " (", document["release/information/region"].text(), ")",
    " (", document["release/information/revision"].text(), ")",
    ".st/"
  };
  directory::create(pathname);

  //strip "release" root node from database entry (since a single game manifest isn't part of a database)
  string markup = manifest;
  markup.replace("\n  ", "\n");
  markup.replace("information", "\ninformation");
  markup.ltrim<1>("release\n");

  file::write({pathname, "manifest.bml"}, markup);
  file::write({pathname, "program.rom"}, buffer);
  copySufamiTurboSaves(pathname);

  return pathname;
}

string Ananke::createSufamiTurboHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    userpath(), "Emulation/Sufami Turbo/",
    nall::basename(information.name),
    " (!).st/"
  };
  directory::create(pathname);

  file::write({pathname, "manifest.bml"}, {
    "cartridge\n",
    "  rom name=program.rom size=0x", hex(buffer.size()), "\n",
    "  ram name=save.ram size=0x2000\n",
    "\n",
    "information\n",
    "  title: ", nall::basename(information.name), "\n"
  });
  file::write({pathname, "program.rom"}, buffer);
  copySufamiTurboSaves(pathname);

  return pathname;
}

string Ananke::openSufamiTurbo(vector<uint8_t> &buffer) {
  string sha256 = nall::sha256(buffer.data(), buffer.size());

  string databaseText = string::read({configpath(), "ananke/database/Sufami Turbo.bml"}).strip();
  if(databaseText.empty()) databaseText = string{Database::SufamiTurbo}.strip();
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);

    if(document["release/information/sha256"].text() == sha256) {
      return createSufamiTurboDatabase(buffer, document, item);
    }
  }

  return createSufamiTurboHeuristic(buffer);
}

string Ananke::syncSufamiTurbo(const string &pathname) {
  auto buffer = file::read({pathname, "program.rom"});
  if(buffer.size() == 0) return "";

  auto save = file::read({pathname, "save.ram"});
  if(save.size() == 0) save = file::read({pathname, "save.rwm"});

  directory::remove(pathname);
  string outputPath = openSufamiTurbo(buffer);

  if(save.size()) file::write({outputPath, "save.ram"}, save);

  return outputPath;
}
