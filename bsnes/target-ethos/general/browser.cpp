Browser *browser = nullptr;

Browser::Browser() {
  setGeometry({128, 128, 640, 400});

  layout.setMargin(5);
  pathBrowse.setText("Browse ...");
  pathUp.setText("..");
  openButton.setText("Open");

  append(layout);
  layout.append(pathLayout, {~0, 0}, 5);
    pathLayout.append(pathEdit, {~0, 0}, 5);
    pathLayout.append(pathBrowse, {0, 0}, 5);
    pathLayout.append(pathUp, {0, 0});
  layout.append(fileList, {~0, ~0}, 5);
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(filterLabel, {~0, 0}, 5);
    controlLayout.append(openButton, {80, 0});

  pathEdit.onActivate = [&] {
    string path = pathEdit.text();
    path.transform("\\", "/");
    if(path.endswith("/") == false) path.append("/");
    setPath(path);
  };

  pathBrowse.onActivate = [&] {
    string path = DialogWindow::folderSelect(*this, this->path);
    if(!path.empty()) setPath(path);
  };

  pathUp.onActivate = [&] {
    if(this->path == "/") return;
    string path = this->path;
    path.rtrim<1>("/");
    path = dir(path);
    setPath(path);
  };

  fileList.onChange = {&Browser::synchronize, this};
  fileList.onActivate = openButton.onActivate = {&Browser::fileListActivate, this};

  synchronize();
}

void Browser::open(Emulator::Interface::Media &media, function<void (string)> callback) {
  this->media = media;
  this->callback = callback;

  setTitle({"Load ", media.displayname});
  setPath("/media/sdb1/root/cartridges/Game Boy Advance/");

  filterLabel.setText({"Files of type: ", media.filter});

  setVisible();
}

void Browser::synchronize() {
  openButton.setEnabled(fileList.selected());
}

void Browser::setPath(const string &path) {
  this->path = path;
  pathEdit.setText(path);

  fileList.reset();
  filenameList.reset();

  lstring contents = directory::contents(path);
  for(auto &filename : contents) {
    if(filename.endswith("/")) {
      filenameList.append(filename);
    } else if(filename.wildcard(media.filter)) {
      filenameList.append(filename);
    }
  }

  for(auto &filename : filenameList) fileList.append(filename);
  fileList.setSelection(0);
  fileList.setFocused();
  synchronize();
}

void Browser::fileListActivate() {
  unsigned selection = fileList.selection();
  string filename = filenameList[selection];
  if(filename.endswith("/")) {
    if(loadFolder({path, filename})) return;
    return setPath({path, filename});
  }
  loadFile({path, filename});
}

bool Browser::loadFolder(const string &path) {
  string requested = path;
  requested.rtrim<1>("/");
  if(requested.wildcard(media.filter) == false) return false;
  loadFile(path);
  return true;
}

void Browser::loadFile(const string &filename) {
  setVisible(false);
  if(callback) callback(filename);
}
