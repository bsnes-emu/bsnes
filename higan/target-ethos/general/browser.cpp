Browser *browser = nullptr;

Browser::Browser() {
  bootstrap();
  setGeometry({128, 128, 640, 400});
  windowManager->append(this, "Browser");

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
  onClose = [&] { dialogActive = false; };

  synchronize();
}

void Browser::synchronize() {
  openButton.setEnabled(fileList.selected());
  if(fileList.selected()) {
    for(auto &folder : folderList) {
      if(folder.extension == extension) {
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
        if(folder.extension == media.type) {
          found = true;
          break;
        }
      }
      if(found == true) continue;

      Folder folder;
      folder.extension = media.type;
      folder.path = application->basepath;
      folder.selection = 0;
      folderList.append(folder);
    }
  }

  for(auto &folder : folderList) {
    config.append(folder.path, folder.extension);
    config.append(folder.selection, string{folder.extension, "::selection"});
  }

  config.load(application->path("paths.cfg"));
  config.save(application->path("paths.cfg"));
}

void Browser::inputEvent(unsigned scancode, int16_t value) {
  if(scancode == keyboard(0)[nall::Keyboard::Escape] && value == 0) onClose();

  //proof of concept only, not very useful:
  //joypad up/down moves around in the file list, any joypad button loads selected game
  if(fileList.selected() == false) return;
  unsigned selection = fileList.selection();

  if(Joypad::isAnyHat(scancode)) {
    if(value & Joypad::HatUp  ) fileList.setSelection(selection - 1);
    if(value & Joypad::HatDown) fileList.setSelection(selection + 1);
  }

  if(Joypad::isAnyButton(scancode) && value == 1) {
    fileList.onActivate();
  }
}

string Browser::select(const string &title, const string &extension) {
  this->extension = extension;

  string path;
  unsigned selection = 0;
  for(auto &folder : folderList) {
    if(folder.extension == extension) {
      path = folder.path;
      selection = folder.selection;
      break;
    }
  }
  if(path.empty()) path = application->basepath;
  setPath(path, selection);

  filterLabel.setText({"Filter: *.", extension});

  audio.clear();
  setTitle(title);
  setModal(true);
  setVisible(true);
  fileList.setFocused();
  outputFilename = "";

  dialogActive = true;
  while(dialogActive == true) {
    OS::processEvents();
    inputManager->poll();
    usleep(20 * 1000);
  }

  setModal(false);
  setVisible(false);
  return outputFilename;
}

void Browser::setPath(const string &path, unsigned selection) {
  //save path for next browser selection
  for(auto &folder : folderList) {
    if(folder.extension == extension) folder.path = path;
  }

  this->path = path;
  pathEdit.setText(path);

  fileList.reset();
  filenameList.reset();

  lstring contents = directory::contents(path);

  for(auto &filename : contents) {
    if(!filename.wildcard(R"(*.??/)") && !filename.wildcard(R"(*.???/)") && filename.endswith("/")) {
      string name = filename;
      name.rtrim<1>("/");
      fileList.append(name);
      fileList.setImage(filenameList.size(), 0, image(resource::systemFolder, sizeof resource::systemFolder));
      filenameList.append(filename);
    }
  }

  for(auto &filename : contents) {
    string suffix = {".", this->extension, "/"};
    if(filename.wildcard(R"(*.??/)") || filename.wildcard(R"(*.???/)")) {
      if(filename.endswith(suffix)) {
        string name = filename;
        name.rtrim<1>(suffix);
        fileList.append(name);
        fileList.setImage(filenameList.size(), 0, image(resource::gameFolder, sizeof resource::gameFolder));
        filenameList.append(filename);
      }
    }
  }

  for(auto &filename : contents) {
    string suffix = {".", this->extension};
    if(filename.wildcard(R"(*.??)") || filename.wildcard(R"(*.???)")) {
      if(filename.endswith(suffix)) {
        string name = filename;
        fileList.append(name);
        fileList.setImage(filenameList.size(), 0, image(resource::gameFile, sizeof resource::gameFile));
        filenameList.append(filename);
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
  if(string{filename}.rtrim<1>("/").endswith(this->extension) == false) return setPath({path, filename});

  dialogActive = false;
  outputFilename = {path, filename};
}
