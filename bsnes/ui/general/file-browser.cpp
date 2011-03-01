FileBrowser fileBrowser;

void FileBrowser::create() {
  application.addWindow(this, "FileBrowser", "160,160");

  browseButton.setText("...");
  upButton.setText("..");

  layout.setMargin(5);
  pathLayout.append(pathBox, 0, 0, 5);
  pathLayout.append(browseButton, Style::LineEditHeight, 0, 5);
  pathLayout.append(upButton, Style::LineEditHeight, 0);
  layout.append(pathLayout, 0, Style::LineEditHeight, 5);
  layout.append(contentsBox, 0, 0);

  setGeometry({ 0, 0, 640, layout.minimumHeight() + 400 });
  append(layout);

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

//if(mode == requestedMode && folder == config.path.current) {
//  setVisible();
//  contentsBox.setFocused();
//  return;
//}

  setVisible(false);
  filters.reset();

  switch(mode = requestedMode) {
    case Mode::Cartridge: {
      setTitle("Load Cartridge");
      folderPath = "sfc";
      filters.append(".sfc");
      break;
    }
    case Mode::Satellaview: {
      setTitle("Load Satellaview Cartridge");
      folderPath = "bs";
      filters.append(".bs");
      break;
    }
    case Mode::SufamiTurbo: {
      setTitle("Load Sufami Turbo Cartridge");
      folderPath = "st";
      filters.append(".st");
      break;
    }
    case Mode::GameBoy: {
      setTitle("Load Game Boy Cartridge");
      folderPath = "gb";
      filters.append(".gb");
      filters.append(".gbc");
      filters.append(".sgb");
      break;
    }
    case Mode::Filter: {
      setTitle("Load Video Filter");
      folderPath = "filter";
      filters.append(".filter");
      break;
    }
    case Mode::Shader: {
      setTitle("Load Pixel Shader");
      folderPath = "shader";
      filters.append(".shader");
      break;
    }
  }

  setFolder(path.load(folderPath));
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
