FileBrowser *fileBrowser = 0;

FileBrowser::FileBrowser() {
  setGeometry({ 128, 128, 640, 400 });
  windowManager->append(this, "FileBrowser");

  layout.setMargin(5);
  pathBrowse.setText("Browse ...");
  pathUp.setText("..");
  openButton.setText("Open");

  append(layout);
    layout.append(pathLayout, ~0, 0, 5);
      pathLayout.append(pathEdit, ~0, 0, 5);
      pathLayout.append(pathBrowse, 0, 0, 5);
      pathLayout.append(pathUp, 0, 0);
    layout.append(fileList, ~0, ~0, 5);
    layout.append(controlLayout, ~0, 0);
      controlLayout.append(filterLabel, ~0, 0, 5);
      controlLayout.append(openButton, 80, 0);

  pathEdit.onActivate = [&] {
    string path = pathEdit.text();
    path.transform("\\", "/");
    if(path.endswith("/") == false) path.append("/");
    setPath(path);
  };

  pathBrowse.onTick = [&] {
    string path = OS::folderSelect(*this, mode->path);
    if(path != "") setPath(path);
  };

  pathUp.onTick = [&] {
    if(mode->path == "/") return;
    string path = mode->path;
    path.rtrim<1>("/");
    path = dir(path);
    setPath(path);
  };

  fileList.onActivate = openButton.onTick = { &FileBrowser::fileListActivate, this };

  filterModes[Mode::Default    ] = { "Default",     "", { "*" } };
  filterModes[Mode::NES        ] = { "NES",         "", { "*.nes" } };
  filterModes[Mode::SNES       ] = { "SNES",        "", { "*.sfc" } };
  filterModes[Mode::GameBoy    ] = { "GameBoy",     "", { "*.gb", "*.gbc" } };
  filterModes[Mode::Satellaview] = { "Satellaview", "", { "*.bs" } };
  filterModes[Mode::SufamiTurbo] = { "SufamiTurbo", "", { "*.st" } };
  mode = &filterModes[Mode::Default];

  for(auto &mode : filterModes) config.attach(mode.path, mode.name);
  config.load(string{ application->userpath, "paths.cfg" });
  config.save(string{ application->userpath, "paths.cfg" });
}

FileBrowser::~FileBrowser() {
  config.save(string{ application->userpath, "paths.cfg" });
}

void FileBrowser::open(const string &title, unsigned requestedMode, function<void (string)> requestedCallback) {
  callback = requestedCallback;
  if(mode == &filterModes[requestedMode]) {
    setVisible();
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

  lstring contentsList = directory::contents(requestedPath);
  lstring fileNameList;
  for(auto &fileName : contentsList) {
    for(auto &filter : mode->filter) {
      if(fileName.wildcard(filter)) {
        fileNameList.append(fileName);
        break;
      }
    }
  }

  if(fileNameList.size() != 1) return false;
  loadFile({ requestedPath, fileNameList[0] });
  return true;
}

void FileBrowser::loadFile(const string &filename) {
  if(callback) callback(filename);
  setVisible(false);
}
