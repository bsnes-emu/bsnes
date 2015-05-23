#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow {
  BrowserDialogWindow(BrowserDialog::State& state) : state(state) {}
  auto accept() -> void;
  auto activate() -> void;
  auto change() -> void;
  auto isFolder(const string& name) -> bool;
  auto run() -> lstring;
  auto setPath(string path) -> void;

private:
  Window window;
    VerticalLayout layout{&window};
      HorizontalLayout pathLayout{&layout, Size{~0, 0}, 8};
        LineEdit pathName{&pathLayout, Size{~0, 0}, 0};
        Button pathHome{&pathLayout, Size{0, 0}, 0};
        Button pathRefresh{&pathLayout, Size{0, 0}, 0};
        Button pathUp{&pathLayout, Size{0, 0}, 0};
      ListView view{&layout, Size{~0, ~0}, 8};
      HorizontalLayout controlLayout{&layout, Size{~0, 0}};
        ComboButton filterList{&controlLayout, Size{120, 0}, 8};
        LineEdit fileName{&controlLayout, Size{~0, 0}, 8};
        Button acceptButton{&controlLayout, Size{80, 0}, 8};
        Button cancelButton{&controlLayout, Size{80, 0}, 8};

  BrowserDialog::State& state;
};

//accept button clicked, or enter pressed on file name line edit
//also called by list view activate after special case handling
auto BrowserDialogWindow::accept() -> void {
  auto selectedItems = view.selectedItems();

  if(state.action == "openFile" && selectedItems) {
    string name = selectedItems.first()->text(0);
    if(isFolder(name)) return setPath({state.path, name});
    state.response.append(string{state.path, name});
  }

  if(state.action == "openFiles") {
    for(auto selectedItem : selectedItems) {
      string name = selectedItem->text(0);
      state.response.append(string{state.path, name, isFolder(name) ? "/" : ""});
    }
  }

  if(state.action == "saveFile") {
    string name = fileName.text();
    if(!name && selectedItems) name = selectedItems.first()->text(0);
    if(!name || isFolder(name)) return;
    if(file::exists({state.path, name})) {
      if(MessageDialog("File already exists; overwrite it?").question() != 0) return;
    }
    state.response.append(string{state.path, name});
  }

  if(state.action == "selectFolder" && selectedItems) {
    string name = selectedItems.first()->text(0);
    if(isFolder(name)) state.response.append(string{state.path, name, "/"});
  }

  if(state.response) window.setModal(false);
}

//list view item double-clicked, or enter pressed on selected list view item
auto BrowserDialogWindow::activate() -> void {
  auto selectedItem = view.selected();
  if(state.action == "saveFile" && selectedItem) {
    string name = selectedItem->text(0);
    if(isFolder(name)) return setPath({state.path, name});
    fileName.setText(isFolder(name) ? "" : name);
  }
  if(state.action == "selectFolder" && selectedItem) {
    string name = selectedItem->text(0);
    if(isFolder(name)) return setPath({state.path, name});
  }
  accept();
}

//list view item changed
auto BrowserDialogWindow::change() -> void {
  fileName.setText("");
  if(state.action == "saveFile") {
    if(auto selectedItem = view.selected()) {
      string name = selectedItem->text(0);
      if(!isFolder(name)) fileName.setText(name);
    }
  }
}

auto BrowserDialogWindow::isFolder(const string& name) -> bool {
  return directory::exists({state.path, name});
}

auto BrowserDialogWindow::run() -> lstring {
  state.response.reset();

  layout.setMargin(8);
  pathName.onActivate([&] { setPath(pathName.text()); });
  pathHome.onActivate([&] { setPath(userpath()); });
  pathHome.setBordered(false).setIcon(Icon::Go::Home);
  pathRefresh.onActivate([&] { setPath(state.path); });
  pathRefresh.setBordered(false).setIcon(Icon::Action::Refresh);
  pathUp.onActivate([&] { setPath(state.path.dirname()); });
  pathUp.setBordered(false).setIcon(Icon::Go::Up);
  view.onActivate([&] { activate(); });
  view.onChange([&] { change(); });
  view.setMultiSelect(state.action == "openFiles");
  filterList.onChange([&] { setPath(state.path); });
  for(auto& filter : state.filters) {
    auto part = filter.split<1>("|");
    filterList.append(ComboButtonItem().setText(part.first()));
  }
  filterList.setVisible(state.action != "selectFolder");
  fileName.onActivate([&] { accept(); });
  fileName.setVisible(state.action == "saveFile");
  acceptButton.onActivate([&] { accept(); });
  if(state.action == "openFile" || state.action == "openFiles") acceptButton.setText("Open");
  if(state.action == "saveFile") acceptButton.setText("Save");
  if(state.action == "selectFolder") acceptButton.setText("Select");
  cancelButton.onActivate([&] { window.setModal(false); });
  cancelButton.setText("Cancel");

  setPath(state.path);

  window.onClose([&] { window.setModal(false); });
  window.onSize([&] { view.resizeColumns(); });
  window.setTitle(state.title);
  window.setSize({640, 480});
  window.setCentered(state.parent);
  window.setVisible();
  view.resizeColumns();
  window.setModal();
  window.setVisible(false);

  return state.response;
}

auto BrowserDialogWindow::setPath(string path) -> void {
  path.transform("\\", "/");
  if(!path.endsWith("/")) path.append("/");
  pathName.setText(state.path = path);

  view.reset();
  view.append(ListViewColumn().setWidth(~0));
  view.append(ListViewColumn().setWidth( 0).setForegroundColor({192, 128, 128}));

  for(auto& folder : directory::folders(path)) {
    ListViewItem item{&view};
    item.setIcon(0, Icon::Emblem::Folder);
    item.setText(0, folder.rtrim("/"));
    item.setText(1, octal<3>(storage::mode({path, folder}) & 0777));
  }

  if(state.action != "selectFolder") {  //don't show files during folder selection
    string filter = "*";
    if(auto selected = filterList.selected()) {
      auto part = state.filters[selected->offset()].split<1>("|");
      filter = part.last();
    }

    for(auto& file : directory::files(path)) {
      if(!file.match(filter)) continue;
      ListViewItem item{&view};
      item.setIcon(0, Icon::Emblem::File);
      item.setText(0, file);
      item.setText(1, octal<3>(storage::mode({path, file}) & 0777));
    }
  }

  if(view.items()) view.setSelected({0});
  Application::processEvents();
  view.resizeColumns().setFocused().doChange();
}

//

BrowserDialog::BrowserDialog() {
}

auto BrowserDialog::openFile() -> string {
  state.action = "openFile";
  if(!state.title) state.title = "Open File";
  if(auto result = _run()) return result.first();
  return {};
}

auto BrowserDialog::openFiles() -> lstring {
  state.action = "openFiles";
  if(!state.title) state.title = "Open Files";
  if(auto result = _run()) return result;
  return {};
};

auto BrowserDialog::saveFile() -> string {
  state.action = "saveFile";
  if(!state.title) state.title = "Save File";
  if(auto result = _run()) return result.first();
  return {};
}

auto BrowserDialog::selectFolder() -> string {
  state.action = "selectFolder";
  if(!state.title) state.title = "Select Folder";
  if(auto result = _run()) return result.first();
  return {};
}

auto BrowserDialog::setFilters(const lstring& filters) -> type& {
  state.filters = filters;
  return *this;
}

auto BrowserDialog::setParent(const shared_pointer<mWindow>& parent) -> type& {
  state.parent = parent;
  return *this;
}

auto BrowserDialog::setPath(const string& path) -> type& {
  state.path = path;
  return *this;
}

auto BrowserDialog::setTitle(const string& title) -> type& {
  state.title = title;
  return *this;
}

auto BrowserDialog::_run() -> lstring {
  if(!state.path) state.path = userpath();
  return BrowserDialogWindow(state).run();
}

#endif
