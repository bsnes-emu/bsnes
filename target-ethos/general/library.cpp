LibraryManager* libraryManager = nullptr;

LibraryBrowser::LibraryBrowser(Emulator::Interface& emulator) : emulator(emulator) {
  setMargin(5);

  informationType.setText({
    "Title:\n",
    "Serial:"
  });

  for(auto& media : emulator.media) {
    mediaMode.append(media.name);
  }

  unsigned height = Font::size(program->normalFont, " ").height;

  append(folders, {~0, ~0}, 5);
  append(informationLayout, {~0, 0});
  informationLayout.append(informationType, {0, height * 2}, 5);
  informationLayout.append(information, {~0, height * 2}, 5);
  informationLayout.append(mediaMode, {0, 0});

  folders.onActivate = {&LibraryBrowser::onActivate, this};
  folders.onChange = {&LibraryBrowser::onChange, this};
  mediaMode.onChange = {&LibraryBrowser::setMode, this};
}

void LibraryBrowser::onActivate() {
  if(folders.selected() == false) return;
  if(libraryManager->loadButton.enabled() == false) return;

  unsigned selection = folders.selection();
  string pathname = {this->pathname, folders.text(selection, 0), typeSuffix};

  libraryManager->loaded.append(folders.text(selection, 0));
  libraryManager->setInformation(false);

  if(libraryManager->slotLoad == false) {
    utility->loadMedia(pathname);
  } else {
    libraryManager->loadPathname = pathname;
    libraryManager->setModal(false);
  }
}

void LibraryBrowser::onChange() {
  if(folders.selected() == false) return information.setText("");

  string manifest = {pathname, folders.text(folders.selection(), 0), typeSuffix, "manifest.bml"};
  auto document = Markup::Document(file::read(manifest));

  information.setText({
    document["information/title"].text(), "\n",
    document["information/serial"].text()
  });
}

void LibraryBrowser::refresh() {
  folders.reset();
  lstring pathnames = directory::ifolders(pathname, typeMask);
  unsigned selection = 0;
  for(auto& pathname : pathnames) {
    folders.append(string{pathname}.rtrim<1>(typeSuffix));
    folders.setImage(selection++, 0, {resource::game, sizeof resource::game});
  }
  folders.setSelection(0);
  onChange();
}

void LibraryBrowser::setMode() {
  auto& media = emulator.media[mediaMode.selection()];

  pathname = {utility->libraryPath(), media.name, "/"};
  type = media.type;
  typeMask = {"*.", type};
  typeSuffix = {".", type, "/"};

  refresh();
  folders.setFocused();
  libraryManager->synchronize();
}

LibraryImport::LibraryImport() {
  setMargin(5);
  information.setText({
    "higan manages games in a library. To play a game, you must first import the game.\n"
    "After doing so, the game will appear inside your library, and can then be loaded and played."
  });
  importButton.setText("Import Game ...");
  append(information, {~0, 0}, 5);
  append(importButton, {0, 0});

  importButton.onActivate = {&LibraryImport::onImportActivate, this};
}

void LibraryImport::onImportActivate() {
  if(program->ananke.open() == false) {
    MessageWindow().setText("ananke must be installed to use this feature").warning();
    return;
  }
  function<string ()> browse = program->ananke.sym("ananke_browse");
  if(!browse) return;
  string pathname = browse();
  if(pathname.empty()) return;
  MessageWindow().setText({"Successfully imported ", notdir(pathname.rtrim<1>("/"))}).information();

  //after importing game, take user to the relevant game list to show the newly imported title
  string type = extension(pathname);
  for(signed bootable = 1; bootable >= 0; bootable--) {
    unsigned selection = 0;
    for(auto& browser : libraryManager->browsers) {
      unsigned mode = 0;
      for(auto& media : browser->emulator.media) {
        if(type == media.type && media.bootable == bootable) {
          browser->mediaMode.setSelection(mode);
          libraryManager->libraryFrame.setSelection(selection);
          libraryManager->onChange();
          return;
        }
        mode++;
      }
      selection++;
    }
  }
}

LibraryManager::LibraryManager() {
  setTitle("Game Library");
  setGeometry({128, 128, 640, 680});
  windowManager->append(this, "LibraryManager");

  layout.setMargin(5);
  bootstrap();
  libraryFrame.append("Import");
  libraryFrame.setLayout(browsers.size(), libraryImport);
  loadButton.setText("Load");

  unsigned height = Font::size(program->normalFont, " ").height;

  append(layout);
  layout.append(libraryFrame, {~0, ~0}, 5);
  layout.append(informationLayout, {~0, 0});
  informationLayout.append(information, {~0, height * 3}, 5);
  informationLayout.append(skipButton, {80, 0}, 5);
  informationLayout.append(loadButton, {80, 0});

  onClose = skipButton.onActivate = [&] {
    setModal(false);
    setVisible(false);
  };

  libraryFrame.onChange = {&LibraryManager::onChange, this};
  loadButton.onActivate = {&LibraryManager::onLoad, this};

  //initial config value of -1 defaults to import tab on first launch of higan
  if(config->library.selection < 0) config->library.selection = browsers.size();
  libraryFrame.setSelection(config->library.selection);
}

void LibraryManager::bootstrap() {
  unsigned selection = 0;
  for(auto& emulator : program->emulator) {
    LibraryBrowser* browser = new LibraryBrowser(*emulator);
    libraryFrame.append(emulator->information.name);
    libraryFrame.setLayout(selection++, *browser);
    browsers.append(browser);
  }
}

string LibraryManager::load(const string& type) {
  requestedLoadType = type;
  unsigned selection = 0;
  for(auto& browser : browsers) {
    unsigned mode = 0;
    for(auto& media : browser->emulator.media) {
      if(type == media.type && media.bootable == false) {
        libraryFrame.setSelection(selection);
        browser->mediaMode.setSelection(mode);
        browser->setMode();

        slotLoad = true;
        loadPathname = "";
        show();
        setModal();
        slotLoad = false;
        browser->mediaMode.setSelection(0);
        return loadPathname;
      }
      mode++;
    }
    selection++;
  }
  return "";  //should never occur
}

void LibraryManager::onChange() {
  unsigned selection = libraryFrame.selection();
  config->library.selection = selection;
  if(selection < browsers.size()) {
    browsers[selection]->setMode();
  } else {
    loadButton.setEnabled(false);
  }
}

void LibraryManager::onLoad() {
  unsigned selection = libraryFrame.selection();
  if(selection < browsers.size()) browsers[selection]->onActivate();
}

void LibraryManager::setInformation(bool load) {
  string text;
  if(loaded.size() == 0) {
    text = {" \nPlease select a game to load ...\n "};
  } else if(loaded.size() == 1 && load == false) {
    text = {" \n", loaded[0], "\n "};
  } else if(loaded.size() == 1 && load == true) {
    text = {loaded[0], " \nPlease select a slot game to load ...\n "};
  } else if(loaded.size() == 2 && load == false) {
    text = {loaded[0], "\n", loaded[1], "\n "};
  } else if(loaded.size() == 2 && load == true) {
    text = {loaded[0], "\n", loaded[1], "\nPlease select a slot game to load ..."};
  } else if(loaded.size() == 3) {
    text = {loaded[0], "\n", loaded[1], "\n", loaded[2]};
  }
  information.setText(text);
}

void LibraryManager::show() {
  if(slotLoad == false) {
    loaded.reset();
    requestedLoadType.reset();
    skipButton.setText("Cancel");
  } else {
    skipButton.setText("Skip");
  }

  setInformation(true);
  setVisible();
  setFocused();
  onChange();
}

//set library to show a specific media type, and then show the library
void LibraryManager::show(const string& type) {
  unsigned selection = 0;
  for(auto& browser : browsers) {
    unsigned mode = 0;
    for(auto& media : browser->emulator.media) {
      if(media.bootable && media.type == type) {
        libraryFrame.setSelection(selection);
        browser->mediaMode.setSelection(mode);
        browser->setMode();
        return show();
      }
      mode++;
    }
    selection++;
  }
}

void LibraryManager::synchronize() {
  if(libraryFrame.selection() < browsers.size()) {
    auto& emulator = browsers[libraryFrame.selection()]->emulator;
    auto& media = emulator.media[browsers[libraryFrame.selection()]->mediaMode.selection()];

    if(requestedLoadType.empty()) {
      loadButton.setEnabled(media.bootable);
    } else {
      loadButton.setEnabled(requestedLoadType == media.type);
    }
  } else {
    loadButton.setEnabled(false);
  }
}
