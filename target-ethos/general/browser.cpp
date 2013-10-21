Browser* browser = nullptr;

Browser::Browser() {
  bootstrap();
  setGeometry({128, 128, 640, 400});
  windowManager->append(this, "Browser");

  layout.setMargin(5);
  homeButton.setImage({resource::home, sizeof resource::home});
  upButton.setImage({resource::up, sizeof resource::up});
  openButton.setText("Open");

  append(layout);
  layout.append(pathLayout, {~0, 0}, 5);
    pathLayout.append(pathEdit, {~0, 0}, 5);
    pathLayout.append(homeButton, {28, 28}, 5);
    pathLayout.append(upButton, {28, 28});
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

  homeButton.onActivate = [&] {
    string libraryPath = string::read({configpath(), "higan/library.bml"}).strip().ltrim<1>("Path: ").transform("\\", "/");
    if(libraryPath.empty()) libraryPath = {userpath(), "Emulation/"};
    if(libraryPath.endswith("/") == false) libraryPath.append("/");
    setPath(libraryPath);
  };

  upButton.onActivate = [&] {
    setPath(parentdir(path));
  };

  fileList.onChange = {&Browser::synchronize, this};
  fileList.onActivate = openButton.onActivate = {&Browser::fileListActivate, this};

  onClose = [&] {
    setModal(false);
    setVisible(false);
  };

  synchronize();
}

void Browser::synchronize() {
  openButton.setEnabled(fileList.selected());
  if(fileList.selected()) {
    for(auto& folder : folderList) {
      if(folder.extension == extension) {
        folder.selection = fileList.selection();
      }
    }
  }
}

void Browser::saveConfiguration() {
  config.save(program->path("paths.bml"));
}

void Browser::bootstrap() {
  for(auto& emulator : program->emulator) {
    for(auto& media : emulator->media) {
      bool found = false;
      for(auto& folder : folderList) {
        if(folder.extension == media.type) {
          found = true;
          break;
        }
      }
      if(found == true) continue;

      Folder folder;
      folder.extension = media.type;
      folder.path = {userpath(), "Emulation/", media.name, "/"};
      folder.selection = 0;
      folderList.append(folder);
    }
  }

  for(auto& folder : folderList) {
    Configuration::Node node;
    node.append(folder.path, "Path");
    node.append(folder.selection, "Selection");
    config.append(node, folder.extension);
  }

  config.load(program->path("paths.bml"));
  config.save(program->path("paths.bml"));
}

string Browser::select(string title, string extension) {
  this->extension = extension;

  string path;
  unsigned selection = 0;
  for(auto& folder : folderList) {
    if(folder.extension == extension) {
      path = folder.path;
      selection = folder.selection;
      break;
    }
  }
  if(path.empty()) path = program->basepath;
  setPath(path, selection);

  filterLabel.setText({"Filter: *.", extension});

  audio.clear();
  setTitle(title);
  setVisible(true);
  fileList.setFocused();
  outputFilename = "";

  setModal();
  return outputFilename;
}

void Browser::setPath(string path, unsigned selection) {
  //save path for next browser selection
  for(auto& folder : folderList) {
    if(folder.extension == extension) folder.path = path;
  }

  this->path = path;
  pathEdit.setText(path);

  fileList.reset();
  filenameList.reset();

  lstring contents = directory::ifolders(path);

  for(auto& filename : contents) {
    string suffix = {".", this->extension, "/"};
    if(filename.endswith("/") && !filename.endswith(suffix)) {
      string name = filename;
      name.rtrim<1>("/");
      fileList.append(name);
      fileList.setImage(filenameList.size(), 0, {resource::folder, sizeof resource::folder});
      filenameList.append(filename);
    }
  }

  for(auto& filename : contents) {
    string suffix = {".", this->extension, "/"};
    if(filename.endswith(suffix)) {
      string name = filename;
      name.rtrim<1>(suffix);
      fileList.append(name);
      if(1 || file::exists({path, filename, "unverified"}) == false) {
        fileList.setImage(filenameList.size(), 0, {resource::game, sizeof resource::game});
      } else {
        //disabled for now due to performance penalty
        fileList.setImage(filenameList.size(), 0, {resource::unverified, sizeof resource::unverified});
      }
      filenameList.append(filename);
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

  outputFilename = {path, filename};
  onClose();
}
