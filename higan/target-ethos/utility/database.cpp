void Utility::loadMediaFromDatabase(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &filename) {
  string sha256 = file::sha256(filename);
  string systemPath = {configpath(), "higan/", emulator->information.name, ".sys/"};
  string outputPath = {config->path.game, emulator->information.name, "/"};

  string databaseText = string::read({systemPath(), "database.bml"}).rtrim("\n");
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);

    if(document["cartridge"]["information"]["sha256"].text() == sha256) {
      string folderPath = {
        outputPath,
        document["cartridge"]["information"]["name"].text(),
        " (", document["cartridge"]["information"]["region"].text(), ")",
        " (", document["cartridge"]["information"]["revision"].text(), ")",
        ".", extension(filename), "/"
      };
      directory::create(folderPath);
      file::write({folderPath, "sha256"}, (const uint8_t*)(const char*)sha256, sha256.length());
      file::write({folderPath, "manifest.bml"}, (const uint8_t*)(const char*)item, item.length());

      auto buffer = file::read(filename);
      unsigned offset = 0;

      for(auto &node : document["cartridge"]["information"]["configuration"]) {
        if(node.name != "rom") continue;
        string name = node["name"].text();
        unsigned size = node["size"].decimal();

        if(file::exists({folderPath, name}) == false) {
          file::write({folderPath, name}, buffer.data() + offset, size);
        }

        offset += size;
      }

      return loadMedia(emulator, media, folderPath);
    }
  }

  MessageWindow::warning(*presentation,
    "Game not found in database; mapping information not available.\n\n"
    "Unable to proceed with emulation."
  );
}
