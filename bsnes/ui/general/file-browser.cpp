FileBrowser fileBrowser;

void FileBrowser::create() {
  application.addWindow(this, "FileBrowser", "160,160");

  browseButton.setText("...");
  upButton.setText("..");

  layout.setMargin(5);
  pathLayout.append(pathBox,      ~0,  0, 5);
  pathLayout.append(browseButton, 25, 25, 5);
  pathLayout.append(upButton,     25, 25   );
  layout.append(pathLayout,               5);
  layout.append(contentsBox,      ~0, ~0   );
  append(layout);
  setGeometry({ 0, 0, 640, layout.minimumGeometry().height + 400 });

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

  switch(requestedMode) {
    case Mode::Cartridge:   folderPath = "sfc"; break;
    case Mode::Satellaview: folderPath = "bs";  break;
    case Mode::SufamiTurbo: folderPath = "st";  break;
    case Mode::GameBoy:     folderPath = "gb";  break;
  }
  string activePath = path.load(folderPath);

  //if path has not changed, do not reload list; this will preserve previously selected file
  if(mode == requestedMode && folder == activePath) {
    setVisible();
    contentsBox.setFocused();
    return;
  }

  setVisible(false);
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
  }

  setFolder(activePath);
  setVisible(true);
  contentsBox.setFocused();
}

void FileBrowser::setFolder(const string &pathname) {
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

  foreach(item, contents) contentsBox.append(item);
  contentsBox.setSelection(0);
  contentsBox.setFocused();
}

void FileBrowser::folderBrowse() {
  string pathname = OS::folderSelect(*this, folder);
  if(pathname != "") setFolder(pathname);
}

void FileBrowser::folderUp() {
  string path = folder;
  path.rtrim<1>("/");
  if(path != "") setFolder(dir(path));
}

void FileBrowser::fileActivate() {
  if(contentsBox.selected() == false) return;
  string filename = contents[contentsBox.selection()];
  if(strend(filename, "/")) {
    string cartridgeName = cartridgeFolder(filename);
    if(cartridgeName == "") {
      setFolder({ folder, filename });
    } else {
      loadFile({ folder, cartridgeName });
    }
  } else {
    loadFile({ folder, filename });
  }
}

string FileBrowser::cartridgeFolder(const string &pathname) {
  if(strend(pathname, ".sfc/") == false) return "";

  lstring list = directory::files(string(folder, "/", pathname));
  string filename;
  foreach(item, list) {
    if(strend(item, ".sfc")) {
      if(filename != "") return "";  //more than one cartridge in this folder
      filename = item;
    }
  }

  return { pathname, filename };
}

void FileBrowser::loadFile(const string &filename) {
  setVisible(false);
  path.save(folderPath, folder);
  if(callback) callback(filename);
}
