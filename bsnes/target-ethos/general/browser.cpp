Browser *browser = nullptr;

Browser::Browser() {
  bootstrap();
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

void Browser::synchronize() {
  openButton.setEnabled(fileList.selected());
  if(fileList.selected()) {
    for(auto &folder : folderList) {
      if(folder.filter == media.filter) {
        folder.selection = fileList.selection();
      }
    }
  }
}

void Browser::saveConfiguration() {
  config.save(application->path("paths.cfg"));
}

void Browser::bootstrap() {
  for(auto &emulator : application->emulator) {
    for(auto &media : emulator->media) {
      bool found = false;
      for(auto &folder : folderList) {
        if(folder.filter == media.filter) {
          found = true;
          break;
        }
      }
      if(found == true) continue;

      Folder folder;
      folder.filter = media.filter;
      folder.path = application->basepath;
      folder.selection = 0;
      folderList.append(folder);
    }
  }

  for(auto &folder : folderList) {
    config.append(folder.path, folder.filter);
    config.append(folder.selection, string{folder.filter, "::selection"});
  }

  config.load(application->path("paths.cfg"));
  config.save(application->path("paths.cfg"));
}

void Browser::open(Emulator::Interface::Media &media, function<void (string)> callback) {
  this->media = media;
  this->callback = callback;

  setTitle({"Load ", media.displayname});

  string path;
  unsigned selection = 0;
  for(auto &folder : folderList) {
    if(folder.filter == media.filter) {
      path = folder.path;
      selection = folder.selection;
      break;
    }
  }
  if(path.empty()) path = application->basepath;
  setPath(path, selection);

  filterLabel.setText({"Files of type: ", media.filter});

  setVisible();
  fileList.setFocused();
}

void Browser::setPath(const string &path, unsigned selection) {
  for(auto &folder : folderList) {
    if(folder.filter == media.filter) folder.path = path;
  }

  this->path = path;
  pathEdit.setText(path);

  fileList.reset();
  filenameList.reset();

  lstring contents = directory::folders(path);

  for(auto &filename : contents) {
    string filter = {media.filter, "/"};
    if(!filename.wildcard(R"(*.??/)") && !filename.wildcard(R"(*.???/)")) {
      string name = filename;
      name.rtrim<1>("/");
      name = {"[ ", name, " ]"};
      filenameList.append(filename);
      fileList.append(name);
    }
  }

  for(auto &filename : contents) {
    string filter = {media.filter, "/"};
    if(filename.wildcard(R"(*.??/)") || filename.wildcard(R"(*.???/)")) {
      if(filename.wildcard(filter)) {
        string name = filename;
        filter.ltrim<1>("*");
        name.rtrim<1>(filter);
        filenameList.append(filename);
        fileList.append(name);
      }
    }
  }

  fileList.setSelection(selection);
  fileList.setFocused();
  synchronize();
}

void Browser::fileListActivate() {
  unsigned selection = fileList.selection();
  string filename = filenameList[selection];
  string filter = {media.filter, "/"};
  if(filename.wildcard(filter) == false) return setPath({path, filename});
  setVisible(false);
  if(callback) callback({path, filename});
}
