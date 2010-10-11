FileBrowser fileBrowser;

void FileBrowser::create() {
  Window::create(0, 0, 256, 256);
  application.addWindow(this, "FileBrowser", "160,160");

  unsigned x = 5, y = 5, height = Style::TextBoxHeight;

  pathBox.create(*this, x, y, 630 - height - height - 10, height);
  browseButton.create(*this, x + 630 - height - height - 5, y, height, height, "...");
  upButton.create(*this, x + 630 - height, y, height, height, ".."); y += height + 5;

  contentsBox.create(*this, x, y, 630, 350); y += 350 + 5;

  setGeometry(0, 0, 640, y);

  pathBox.onActivate = []() {
    string path = fileBrowser.pathBox.text();
    path.transform("\\", "/");
    if(strend(path, "/") == false) path.append("/");
    fileBrowser.setFolder(path);
  };

  browseButton.onTick = { &FileBrowser::folderBrowse, this };
  upButton.onTick = { &FileBrowser::folderUp, this };
  contentsBox.onActivate = { &FileBrowser::fileActivate, this };
}

void FileBrowser::fileOpen(FileBrowser::Mode requestedMode, function<void (string)> requestedCallback) {
  callback = requestedCallback;
  if(mode == requestedMode && folder == config.path.current) {
    setVisible();
    contentsBox.setFocused();
    return;
  }

  filters.reset();
  switch(mode = requestedMode) {
    case Mode::Cartridge: {
      setTitle("Load Cartridge");
      filters.append(".sfc");
      break;
    }
    case Mode::Satellaview: {
      setTitle("Load Satellaview Cartridge");
      filters.append(".bs");
      break;
    }
    case Mode::SufamiTurbo: {
      setTitle("Load Sufami Turbo Cartridge");
      filters.append(".st");
      break;
    }
    case Mode::GameBoy: {
      setTitle("Load Game Boy Cartridge");
      filters.append(".gb");
      filters.append(".gbc");
      filters.append(".sgb");
      break;
    }
    case Mode::Shader: {
      setTitle("Load Pixel Shader");
      filters.append(".shader");
      break;
    }
  }

  setVisible(false);
  setFolder(config.path.current);
  setVisible(true);
  contentsBox.setFocused();
}

void FileBrowser::setFolder(const char *pathname) {
  contentsBox.reset();
  contents.reset();

  folder = pathname;
  folder.transform("\\", "/");
  pathBox.setText(folder);
  lstring contentsList = directory::contents(folder);
  foreach(item, contentsList) {
    if(strend(item, "/")) {
      contents.append(item);
    } else foreach(filter, filters) {
      if(strend(item, filter)) {
        contents.append(item);
        break;
      }
    }
  }
  foreach(item, contents) contentsBox.addItem(item);
  contentsBox.setSelection(0);
  contentsBox.setFocused();
}

void FileBrowser::folderBrowse() {
  string pathname = OS::folderSelect(*this, folder);
  if(pathname != "") setFolder(pathname);
}

void FileBrowser::folderUp() {
  string path = folder;
  path.rtrim_once("/");
  if(path != "") setFolder(dir(path));
}

void FileBrowser::fileActivate() {
  if(auto position = contentsBox.selection()) {
    string filename = contents[position()];
    if(strend(filename, "/")) {
      string cartridgeName = cartridgeFolder(filename);
      if(cartridgeName == "") {
        setFolder(string(folder, filename));
      } else {
        loadFile({ folder, cartridgeName });
      }
    } else {
      loadFile({ folder, filename });
    }
  }
}

string FileBrowser::cartridgeFolder(const char *pathname) {
  if(strend(pathname, ".sfc/") == false) return "";

  lstring list = directory::files(string(folder, "/", pathname));
  string filename;
  foreach(item, list) {
    if(strend(item, ".sfc")) {
      if(filename != "") return "";  //more than one cartridge in this folder
      filename = item;
    }
  }

  return string(pathname, filename);
}

void FileBrowser::loadFile(const string &filename) {
  setVisible(false);
  config.path.current = folder;
  if(callback) callback(filename);
}
