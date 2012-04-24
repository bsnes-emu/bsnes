bool InterfaceCore::loadFirmware(string filename, string keyname, uint8_t *targetdata, unsigned targetsize) {
  bool result = false;

  filename = application->path(filename);
  string markup;
  markup.readfile(filename);
  XML::Document document(markup);
  lstring keypart = keyname.split<1>(".");

  if(document[keypart[0]][keypart[1]].exists()) {
    auto &key = document[keypart[0]][keypart[1]];
    string firmware = key["firmware"].data;
    string hash = key["sha256"].data;

    uint8_t *data;
    unsigned size;
    if(file::read({dir(filename),firmware}, data, size) == true) {
      if(nall::sha256(data, size) == hash) {
        memcpy(targetdata, data, min(targetsize, size));
        result = true;
      } else {
        MessageWindow::information(Window::None, {"Warning: Firmware SHA256 sum is incorrect:\n\n", dir(filename), firmware});
      }
      delete[] data;
    }
  }

  return result;
}
