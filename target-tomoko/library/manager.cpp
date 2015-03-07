LibraryManager* libraryManager = nullptr;

LibraryManager::LibraryManager() {
  libraryManager = this;

  layout.setMargin(5);

  for(auto& emulator : program->emulators) {
    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      auto browser = new LibraryBrowser(libraryFrame, media);
      libraryBrowsers.append(browser);
    }
  }

  setTitle("Library");
  setSize({640, 800});
  setPlacement(0.0, 0.0);
}

auto LibraryManager::show(const string& type) -> void {
  for(auto& browser : libraryBrowsers) {
    if(type != browser->media.type) continue;
    browser->select();
  }

  setVisible();
  setFocused();
}
