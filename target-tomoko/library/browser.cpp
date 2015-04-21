LibraryBrowser::LibraryBrowser(TabFrame& parent, Emulator::Interface::Media& media) : TabFrameItem{&parent} {
  this->media = media;
  setText(media.name);
  layout.setMargin(5);
  gameList.onActivate([&] {
    libraryManager->setVisible(false);
    program->loadMedia({config().library.location, this->media.name, "/", gameList.selected()->text(), ".", this->media.type, "/"});
  });
}

auto LibraryBrowser::reload() -> void {
  string path = {config().library.location, media.name};
  directory::create(path);

  gameList.reset();
  gameList.append(ListViewColumn());
  bool first = true;
  auto folders = directory::folders(path, {"*.", media.type});
  for(auto& folder : folders) {
    ListViewItem item;
    item.setIcon(0, Icon::Emblem::Program);
    item.setText(folder.rtrim({".", media.type, "/"}));
    gameList.append(item);
    if(first) {
      first = false;
      item.setFocused();
    }
  }
}

auto LibraryBrowser::select() -> void {
  reload();
  setSelected();
  gameList.setFocused();
}
