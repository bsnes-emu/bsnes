LibraryManager* libraryManager = nullptr;

LibraryBrowser::LibraryBrowser() {
  setMargin(5);

  informationType.setText({
    "Title:\n",
    "Revision:\n",
    "Region:\n",
    "Serial:"
  });

  append(folders, {~0, ~0}, 5);
  append(informationLayout, {~0, Font::size(program->normalFont, " ").height * 4});
  informationLayout.append(informationType, {0, ~0}, 5);
  informationLayout.append(information, {~0, ~0});

  folders.onActivate = {&LibraryBrowser::onActivate, this};
  folders.onChange = {&LibraryBrowser::setInformation, this};
}

void LibraryBrowser::onActivate() {
  if(folders.selected() == false) return;
  unsigned selection = folders.selection();
  string pathname = {this->pathname, folders.text(selection, 0), filterSuffix};

  if(libraryManager->slotLoad == false) {
    libraryManager->setStatusText(folders.text(selection, 0));
    utility->loadMedia(pathname);
  } else {
    libraryManager->setStatusText({libraryManager->statusText(), " + ", folders.text(selection, 0)});
    libraryManager->setModal(false);
    libraryManager->loadPathname = pathname;
  }
}

void LibraryBrowser::refresh() {
  folders.reset();
  lstring pathnames = directory::ifolders(pathname, filterMask);
  unsigned selection = 0;
  for(auto& pathname : pathnames) {
    folders.append(string{pathname}.rtrim<1>(filterSuffix));
    folders.setImage(selection++, 0, {resource::game, sizeof resource::game});
  }
}

void LibraryBrowser::setFilter(const string& filter) {
  this->filter = filter;
  filterMask = {"*.", filter};
  filterSuffix = {".", filter, "/"};
}

void LibraryBrowser::setInformation() {
  if(folders.selected() == false) {
    information.setText("");
  } else {
    string manifest = {pathname, folders.text(folders.selection(), 0), filterSuffix, "manifest.bml"};
    auto document = Markup::Document(file::read(manifest));
    information.setText({
      document["information/title"].text(), "\n",
      document["information/revision"].text(), "\n",
      document["information/region"].text(), "\n",
      document["information/serial"].text(), "\n"
    });
  }
}

void LibraryBrowser::setPath(const string& pathname) {
  this->pathname = pathname;
  refresh();
}

LibraryManager::LibraryManager() {
  setTitle("Game Library");
  setStatusVisible();
  setGeometry({128, 128, 960, 640});
  windowManager->append(this, "LibraryManager");

  layout.setMargin(5);
  libraryFrame.append("Import Games");
  importLayout.setMargin(5);
  importInformation.setText({
    "higan manages games in a library. To play a game, you must first import the game.\n"
    "After doing so, the game will appear inside your library, and can then be loaded and played."
  });
  importButton.setText("Import Game ...");
  libraryFrame.setLayout(0, importLayout);
  bootstrap();
  libraryFrame.setSelection(config->library.selection);

  append(layout);
  layout.append(libraryFrame, {~0, ~0});
  importLayout.append(importInformation, {0, 0}, 5);
  importLayout.append(importButton, {0, 0});

  onClose = [&] {
    setModal(false);
    setVisible(false);
  };

  libraryFrame.onChange = [&] {
    config->library.selection = libraryFrame.selection();
  };

  importButton.onActivate = [&] {
    if(program->ananke.open() == false) {
      MessageWindow().setText("ananke must be installed to use this feature").warning();
      return;
    }
    function<string ()> browse = program->ananke.sym("ananke_browse");
    if(!browse) return;
    string pathname = browse();
    if(pathname.empty()) return;
    MessageWindow().setText({"Successfully imported ", notdir(pathname.rtrim<1>("/"))}).information();
    string type = extension(pathname);

    unsigned selection = 1;
    for(auto& browser : browsers) {
      if(browser->filter == type) {
        browser->refresh();
        libraryFrame.setSelection(selection);
        break;
      }
      selection++;
    }
  };
}

void LibraryManager::bootstrap() {
  unsigned selection = 1;
  string basepath = utility->libraryPath();
  vector<string> names;

  for(auto& emulator : program->emulator) {
    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      if(names.find(media.name)) continue;
      names.append(media.name);
      LibraryBrowser* browser = new LibraryBrowser;
      browser->setFilter(media.type);
      browser->setPath({basepath, media.name, "/"});
      libraryFrame.append(media.name);
      libraryFrame.setLayout(selection++, *browser);
      browsers.append(browser);
    }
  }

  for(auto& emulator : program->emulator) {
    for(auto& media : emulator->media) {
      if(media.bootable == true) continue;
      if(names.find(media.name)) continue;
      names.append(media.name);
      LibraryBrowser* browser = new LibraryBrowser;
      browser->setFilter(media.type);
      browser->setPath({basepath, media.name, "/"});
      libraryFrame.append(media.name);
      libraryFrame.setLayout(selection++, *browser);
      browsers.append(browser);
    }
  }
}

string LibraryManager::load(const string& type) {
  setFocused();

  unsigned selection = 1;
  for(auto& browser : browsers) {
    if(browser->filter == type) {
      libraryFrame.setSelection(selection);
      break;
    }
    selection++;
  }

  slotLoad = true;
  loadPathname = "";
  setModal(true);
  slotLoad = false;
  return loadPathname;
}

void LibraryManager::setVisible(bool visible) {
  setStatusText("");
  Window::setVisible(visible);
}
