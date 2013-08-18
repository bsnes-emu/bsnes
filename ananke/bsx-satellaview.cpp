string Ananke::createBsxSatellaviewDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest) {
  string pathname = {
    libraryPath, "BS-X Satellaview/",
    document["release/information/name"].text(),
    " (", document["release/information/region"].text(), ")",
    " (", document["release/information/revision"].text(), ")",
    ".bs/"
  };
  directory::create(pathname);

  //strip "release" root node from database entry (since a single game manifest isn't part of a database)
  string markup = manifest;
  markup.replace("\n  ", "\n");
  markup.replace("information", "\ninformation");
  markup.ltrim<1>("release\n");

  file::write({pathname, "manifest.bml"}, markup);
  file::write({pathname, "program.rom"}, buffer);

  return pathname;
}

string Ananke::createBsxSatellaviewHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    libraryPath, "BS-X Satellaview/",
    nall::basename(information.name),
    ".bs/"
  };
  directory::create(pathname);

  file::write({pathname, "manifest.bml"}, {
    "unverified\n",
    "\n",
    "cartridge\n",
    "  rom name=program.rom size=0x", hex(buffer.size()), " type=FlashROM\n",
    "\n",
    "information\n",
    "  title: ", nall::basename(information.name), "\n"
  });
  file::write({pathname, "program.rom"}, buffer);

  return pathname;
}

string Ananke::openBsxSatellaview(vector<uint8_t> &buffer) {
  string sha256 = nall::sha256(buffer.data(), buffer.size());

  string databaseText = string::read({configpath(), "ananke/database/BS-X Satellaview.bml"}).strip();
  if(databaseText.empty()) databaseText = string{Database::BsxSatellaview}.strip();
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);

    if(document["release/information/sha256"].text() == sha256) {
      return createBsxSatellaviewDatabase(buffer, document, item);
    }
  }

  return createBsxSatellaviewHeuristic(buffer);
}

string Ananke::syncBsxSatellaview(const string &pathname) {
  auto buffer = file::read({pathname, "program.rom"});
  if(buffer.size() == 0) return "";

  directory::remove(pathname);
  information.path = pathname;
  information.name = notdir(string{pathname}.rtrim<1>("/"));
  return openBsxSatellaview(buffer);
}
