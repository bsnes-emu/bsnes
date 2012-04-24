FileBrowser *fileBrowser = nullptr;

FileBrowser::FileBrowser() {
  setGeometry({ 128, 128, 640, 400 });
  windowManager->append(this, "FileBrowser");

  layout.setMargin(5);
  pathBrowse.setText("Browse ...");
  pathUp.setText("..");
  openButton.setText("Open");

  append(layout);
    layout.append(pathLayout, { ~0, 0 }, 5);
      pathLayout.append(pathEdit, { ~0, 0 }, 5);
      pathLayout.append(pathBrowse, { 0, 0 }, 5);
      pathLayout.append(pathUp, { 0, 0 });
    layout.append(fileList, { ~0, ~0 }, 5);
    layout.append(controlLayout, { ~0, 0 });
      controlLayout.append(filterLabel, { ~0, 0 }, 5);
      controlLayout.append(openButton, { 80, 0 });

  pathEdit.onActivate = [&] {
    string path = pathEdit.text();
    path.transform("\\", "/");
    if(path.endswith("/") == false) path.append("/");
    setPath(path);
  };

  pathBrowse.onActivate = [&] {
    string path = DialogWindow::folderSelect(*this, mode->path);
    if(path != "") setPath(path);
  };

  pathUp.onActivate = [&] {
    if(mode->path == "/") return;
    string path = mode->path;
    path.rtrim<1>("/");
    path = dir(path);
    setPath(path);
  };

  fileList.onChange = { &FileBrowser::synchronize, this };
  fileList.onActivate = openButton.onActivate = { &FileBrowser::fileListActivate, this };

  filterModes.append({ "Default",        "", { "*"              } });
  filterModes.append({ "Famicom",        "", { "*.fc",  "*.nes" } });
  filterModes.append({ "SuperFamicom",   "", { "*.sfc"          } });
  filterModes.append({ "GameBoy",        "", { "*.gb",  "*.gbb" } });
  filterModes.append({ "GameBoyColor",   "", { "*.gbc", "*.gbb" } });
  filterModes.append({ "GameBoyAdvance", "", { "*.gba"          } });
  filterModes.append({ "Satellaview",    "", { "*.bs"           } });
  filterModes.append({ "SufamiTurbo",    "", { "*.st"           } });
  mode = &filterModes[Mode::Default];

  for(auto &mode : filterModes) config.attach(mode.path, mode.name);
  config.load(application->path("paths.cfg"));
  config.save(application->path("paths.cfg"));
  synchronize();
}

void FileBrowser::synchronize() {
  openButton.setEnabled(fileList.selected());
}

FileBrowser::~FileBrowser() {
  config.save(application->path("paths.cfg"));
}

void FileBrowser::open(const string &title, unsigned requestedMode, function<void (string)> requestedCallback) {
  callback = requestedCallback;
  if(mode == &filterModes[requestedMode]) {
    setVisible();
    fileList.setFocused();
    return;
  }
  mode = &filterModes[requestedMode];

  setTitle(title);
  setPath(mode->path);

  string filterText = "Files of type: ";
  for(auto &filter : mode->filter) filterText.append(filter, ", ");
  filterText.trim<1>(", ");
  filterLabel.setText(filterText);

  setVisible();
  fileList.setFocused();
}

void FileBrowser::setPath(const string &path) {
  mode->path = path;
  if(mode->path == "") mode->path = application->basepath;
  pathEdit.setText(mode->path);

  fileList.reset();
  fileNameList.reset();

  lstring contentsList = directory::contents(path);
  for(auto &fileName : contentsList) {
    if(fileName.endswith("/")) {
      fileNameList.append(fileName);
    } else for(auto &filter : mode->filter) {
      if(fileName.wildcard(filter)) {
        fileNameList.append(fileName);
        break;
      }
    }
  }

  for(auto &fileName : fileNameList) fileList.append(fileName);
  fileList.setSelection(0);
  fileList.setFocused();
  synchronize();
}

void FileBrowser::fileListActivate() {
  unsigned selection = fileList.selection();
  string fileName = fileNameList[selection];
  if(fileName.endswith("/")) {
    if(loadFolder({ mode->path, fileName })) return;
    return setPath({ mode->path, fileName });
  }
  loadFile({ mode->path, fileName });
}

bool FileBrowser::loadFolder(const string &requestedPath) {
  bool accept = false;
  string path = requestedPath;
  path.rtrim<1>("/");
  for(auto &filter : mode->filter) {
    if(path.wildcard(filter)) accept = true;
  }
  if(accept == false) return false;
  loadFile(requestedPath);
  return true;
}

void FileBrowser::loadFile(const string &filename) {
  setVisible(false);
  if(callback) callback(filename);
}
