FileBrowser fileBrowser;

void FileBrowser::create() {
  application.windows.append(this);
  Window::create(0, 0, 256, 256);
  setDefaultFont(application.proportionalFont);

  unsigned x = 5, y = 5, height = Style::TextBoxHeight;

  pathBox.create(*this, x, y, 630 - height - height - 10, height);
  browseButton.create(*this, x + 630 - height - height - 5, y, height, height, "...");
  upButton.create(*this, x + 630 - height, y, height, height, ".."); y += height + 5;

  contentsBox.create(*this, x, y, 630, 350); y += 350 + 5;

  setGeometry(160, 160, 640, y);

  pathBox.onActivate = []() { fileBrowser.setFolder(fileBrowser.pathBox.text()); };
  browseButton.onTick = { &FileBrowser::folderBrowse, this };
  upButton.onTick = { &FileBrowser::folderUp, this };
  contentsBox.onActivate = { &FileBrowser::fileActivate, this };
}

void FileBrowser::fileOpen(FileBrowser::Mode requestedMode, function<void (string)> requestedCallback) {
  callback = requestedCallback;
  if(mode == requestedMode && string(folder, "/") == config.path.current) {
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
  folder.rtrim("/");
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
  string pathname = os.folderSelect(*this, folder);
  if(pathname != "") setFolder(pathname);
}

void FileBrowser::folderUp() {
  setFolder(dir(folder));
}

void FileBrowser::fileActivate() {
  if(auto position = contentsBox.selection()) {
    string filename = contents[position()];
    if(strend(filename, "/")) {
      setFolder(string(folder, "/", filename));
    } else {
      setVisible(false);
      filename = string(folder, "/", filename);
      config.path.current = dir(filename);
      if(callback) callback(filename);
    }
  }
}
