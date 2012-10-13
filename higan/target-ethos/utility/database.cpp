void Utility::loadMediaFromDatabase(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &filename) {
  string sha256 = file::sha256(filename);
  string systemPath = {configpath(), "higan/", emulator->information.name, ".sys/"};
  string outputPath = {config->path.game, emulator->information.name, "/"};

  string databaseText = string::read({systemPath, "database.bml"}).rtrim("\n");
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);
    if(document["cartridge"]["information"]["sha256"].content() == sha256) {
      string name = {document["cartridge"]["information"]["name"].content(), ".", extension(filename), "/"};
      directory::create({outputPath, name});
      file::write({outputPath, name, "manifest.bml"}, (const uint8_t*)(const char*)item, item.length());
      auto buffer = file::read(filename);
      unsigned offset = 0;
      for(auto &node : document["cartridge"]["layout"]) {
        if(node.name != "file") continue;
        string filename = node["name"].content();
        unsigned filesize = numeral(node["size"].content());
        file::write({outputPath, name, filename}, buffer.data() + offset, filesize);
        offset += filesize;
      }
      return loadMedia(emulator, media, {outputPath, name});
    }
  }

  MessageWindow::warning(*presentation,
    "Game not found in database. Mapping information unavailable.\n\n"
    "As such, emulation cannot proceed."
  );
}
