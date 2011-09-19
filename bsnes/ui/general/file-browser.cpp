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
    string path = OS::folderSelect(*this, activePath);
    if(path != "") setPath(path);
  };

  pathUp.onTick = [&] {
    if(activePath == "/") return;
    string path = activePath;
    path.rtrim<1>("/");
    path = dir(path);
    setPath(path);
  };

  fileList.onActivate = openButton.onTick = { &FileBrowser::fileListActivate, this };

  setPath(config->path.last);
}

void FileBrowser::open(const string &title, const lstring &filterList, function<void (string)> callback) {
  this->callback = callback;
  this->filterList = filterList;

  setTitle(title);
  setPath(activePath);

  string filterText = "Files of type: ";
  foreach(filter, filterList) filterText.append(filter, ", ");
  filterText.trim<1>(", ");
  filterLabel.setText(filterText);

  setVisible();
}

void FileBrowser::setPath(const string &path) {
  config->path.last = path;
  activePath = path;
  pathEdit.setText(activePath);

  fileList.reset();
  fileNameList.reset();

  lstring contentsList = directory::contents(path);
  foreach(fileName, contentsList) {
    if(fileName.endswith("/")) {
      fileNameList.append(fileName);
    } else foreach(filter, filterList) {
      if(fileName.wildcard(filter)) fileNameList.append(fileName);
    }
  }

  foreach(fileName, fileNameList) fileList.append(fileName);
  fileList.setSelection(0);
  fileList.setFocused();
}

void FileBrowser::fileListActivate() {
  unsigned selection = fileList.selection();
  string fileName = fileNameList[selection];
  if(fileName.endswith("/")) return setPath({ activePath, fileName });

  if(callback) callback({ activePath, fileName });
  setVisible(false);
}
