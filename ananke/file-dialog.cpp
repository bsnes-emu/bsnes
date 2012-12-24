struct FileDialog : Window {
  VerticalLayout layout;
    HorizontalLayout pathLayout;
      LineEdit pathEdit;
      Button homeButton;
      Button upButton;
    ListView fileList;
    HorizontalLayout controlLayout;
      Widget spacer;
      Button openButton;

  string open() {
    setModal();
    setVisible();
    fileList.setFocused();
    filename = "";

    dialogActive = true;
    while(dialogActive) {
      OS::processEvents();
      if(Keyboard::pressed(Keyboard::Scancode::Escape)) onClose();
    }

    return filename;
  }

  void setPath(const string &path) {
    pathname = string{path}.transform("\\", "/");
    if(pathname.empty()) pathname = userpath();
    if(pathname.endswith("/") == false) pathname.append("/");
    pathEdit.setText(pathname);

    fileList.reset();
    filenameList.reset();

    lstring folders = directory::ifolders(pathname);
    for(auto &folder : folders) {
      fileList.append(string{folder}.rtrim<1>("/"));
      fileList.setImage(filenameList.size(), 0, {resource::folder, sizeof resource::folder});
      filenameList.append({pathname, folder});
    }

    lstring files = directory::ifiles(pathname);
    for(auto &file : files) {
      fileList.append(file);
      fileList.setImage(filenameList.size(), 0, {resource::file, sizeof resource::file});
      filenameList.append({pathname, file});
    }

    fileList.setSelection(0);
    fileList.setSelected();
    fileList.setFocused();
  }

  FileDialog() {
    setGeometry({64, 64, 480, 600});
    setTitle("Load Image");

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
      controlLayout.append(spacer, {~0, 0});
      controlLayout.append(openButton, {80, 0});

    pathEdit.onActivate = [&] {
      string path = pathEdit.text();
      setPath(path);
    };

    homeButton.onActivate = [&] {
      setPath(userpath());
    };

    upButton.onActivate = [&] {
      setPath(parentdir(pathname));
    };

    fileList.onActivate = openButton.onActivate = [&] {
      if(fileList.selected() == false) return;
      string name = filenameList(fileList.selection());
      if(name.empty()) return;

      if(name.endswith("/")) return setPath(name);
      filename = name;
      onClose();
    };

    onClose = [&] {
      dialogActive = false;
      setModal(false);
      setVisible(false);
    };
  }

private:
  bool dialogActive;
  string pathname;
  string filename;
  lstring filenameList;
};
