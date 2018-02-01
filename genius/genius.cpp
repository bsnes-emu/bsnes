#include <nall/nall.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

#include "genius.hpp"
unique_pointer<ListWindow> listWindow;
unique_pointer<GameWindow> gameWindow;
unique_pointer<MemoryWindow> memoryWindow;

//

ListWindow::ListWindow() {
  listWindow = this;

  fileMenu.setText("File");
  newAction.setText("New").onActivate([&] { newDatabase(); });
  openAction.setText("Open ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*this).setFilters({"*.bml"}).openFile()) {
      loadDatabase(location);
    }
  });
  saveAction.setText("Save").onActivate([&] {
    if(!location) return saveAsAction.doActivate();
    saveDatabase(location);
  });
  saveAsAction.setText("Save As ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*this).setFilters({"*.bml"}).saveFile()) {
      saveDatabase(location);
    }
  });
  quitAction.setText("Quit").onActivate([&] { quit(); });

  helpMenu.setText("Help");
  aboutAction.setText("About ...").onActivate([&] {
    MessageDialog().setParent(*this).setTitle("About").setText({
      "genius v01\n",
      "Author: byuu\n",
      "Website: https://byuu.org/"
    }).information();
  });

  layout.setMargin(5);
  gameList.onActivate([&] { modifyButton.doActivate(); });
  gameList.onChange([&] { updateWindow(); });
  appendButton.setText("Append").onActivate([&] {
    setEnabled(false);
    gameWindow->show();
  });
  modifyButton.setText("Modify").onActivate([&] {
    if(auto item = gameList.selected()) {
      setEnabled(false);
      gameWindow->show(games[item.offset()]);
    }
  });
  removeButton.setText("Remove").onActivate([&] { removeGame(); });

  onClose([&] { quit(); });

  setSize({960, 600});
  reloadList();
  updateWindow();
  setCentered();
  setVisible();
}

auto ListWindow::quit() -> void {
  if(!modified || MessageDialog().setParent(*this).setText({
    "Are you sure you want to quit without saving your changes?"
  }).question() == "Yes") {
    Application::quit();
  }
}

auto ListWindow::reloadList() -> void {
  gameList.reset();
  gameList.append(TableViewHeader()
    .append(TableViewColumn().setText("Name").setExpandable())
    .append(TableViewColumn().setText("Region"))
    .append(TableViewColumn().setText("Revision"))
    .append(TableViewColumn().setText("Board"))
  );
  for(auto& game : games) {
    gameList.append(TableViewItem()
      .append(TableViewCell().setText(game.name))
      .append(TableViewCell().setText(game.region))
      .append(TableViewCell().setText(game.revision))
      .append(TableViewCell().setText(game.board))
    );
  }
  Application::processEvents();
  gameList.resizeColumns();
}

auto ListWindow::updateWindow() -> void {
  modifyButton.setEnabled((bool)gameList.selected());
  removeButton.setEnabled((bool)gameList.selected());
  string name = Location::base(location);
  if(!name) name = "(Untitled)";
  setTitle({modified ? "*" : "", name, " [", games.size(), "] - genius"});
}

auto ListWindow::newDatabase() -> void {
  games.reset();
  modified = false;
  location = "";
  reloadList();
  updateWindow();
}

auto ListWindow::loadDatabase(string location) -> void {
  auto document = BML::unserialize(string::read(location));

  games.reset();
  for(auto node : document.find("game")) {
    Game game;
    game.sha256 = node["sha256"].text();
    game.region = node["region"].text();
    game.revision = node["revision"].text();
    game.board = node["board"].text();
    game.name = node["name"].text();
    game.label = node["label"].text();
    game.note = node["note"].text();
    for(auto leaf : node.find("memory")) {
      Memory memory;
      memory.type = leaf["type"].text();
      memory.size = leaf["size"].text();
      memory.name = leaf["name"].text();
      game.memories.append(memory);
    }
    games.append(game);
  }

  modified = false;
  this->location = location;
  reloadList();
  updateWindow();
}

auto ListWindow::saveDatabase(string location) -> void {
  file fp{location, file::mode::write};
  if(!fp) return MessageDialog().setParent(*this).setText({
    "Error: failed to write file.\n\n",
    "Name: ", location
  }).error(), void();

  auto copy = games;
  copy.sort([](auto x, auto y) {
    return string::icompare(x.name, y.name) < 0;
  });

  fp.print("database\n");
  fp.print("  revision: ", chrono::local::date(), "\n\n");

  for(auto& game : copy) {
    fp.print("game\n");
    fp.print("  sha256:   ", game.sha256, "\n");
    fp.print("  region:   ", game.region, "\n");
    fp.print("  revision: ", game.revision, "\n");
  if(game.board)
    fp.print("  board:    ", game.board, "\n");
    fp.print("  name:     ", game.name, "\n");
  if(game.label)
    fp.print("  label:    ", game.label, "\n");
  if(game.note)
    fp.print("  note:     ", game.note, "\n");
    for(auto& memory : game.memories) {
      fp.print("  memory\n");
      fp.print("    type: ", memory.type, "\n");
      fp.print("    size: ", memory.size, "\n");
      fp.print("    name: ", memory.name, "\n");
    }
    fp.print("\n");
  }

  modified = false;
  this->location = location;
  updateWindow();
}

auto ListWindow::appendGame(Game game) -> void {
  modified = true;
  auto offset = games.size();
  games.append(game);
  reloadList();
  gameList.item(offset).setSelected().setFocused();
  updateWindow();
}

auto ListWindow::modifyGame(Game game) -> void {
  if(auto item = gameList.selected()) {
    modified = true;
    auto offset = item.offset();
    games[offset] = game;
    reloadList();
    gameList.item(offset).setSelected().setFocused();
    updateWindow();
  }
}

auto ListWindow::removeGame() -> void {
  if(auto item = gameList.selected()) {
    if(MessageDialog().setParent(*this).setText({
      "Are you sure you want to permanently remove this game?\n\n",
      "Name: ", item.cell(0).text()
    }).question() == "Yes") {
      modified = true;
      games.remove(item.offset());
      reloadList();
      updateWindow();
    }
  }
}

//

GameWindow::GameWindow() {
  gameWindow = this;

  layout.setMargin(5);
  hashLabel.setText("SHA256:");
  hashEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  regionLabel.setText("Region:");
  regionEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  revisionLabel.setText("Revision:");
  revisionEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  boardLabel.setText("Board:");
  boardEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  nameLabel.setText("Name:");
  nameEdit.onChange([&] { modified = true, updateWindow(); });
  labelLabel.setText("Label:");
  labelEdit.onChange([&] { modified = true, updateWindow(); });
  noteLabel.setText("Note:");
  noteEdit.onChange([&] { modified = true, updateWindow(); });
  memoryList.onActivate([&] { modifyButton.doActivate(); });
  memoryList.onChange([&] { updateWindow(); });
  appendButton.setText("Append").onActivate([&] {
    setEnabled(false);
    memoryWindow->show();
  });
  modifyButton.setText("Modify").onActivate([&] {
    if(auto item = memoryList.selected()) {
      setEnabled(false);
      memoryWindow->show(game.memories[item.offset()]);
    }
  });
  removeButton.setText("Remove").onActivate([&] { removeMemory(); });
  acceptButton.setText("Accept").onActivate([&] { accept(); });
  cancelButton.setText("Cancel").onActivate([&] { cancel(); });

  onClose([&] { cancel(); });

  setSize({800, 480});
  setDismissable();
}

auto GameWindow::show(Game game) -> void {
  this->game = game;
  modified = false;
  create = !game.sha256;

  hashEdit.setText(game.sha256).setEditable(create);
  regionEdit.setText(game.region);
  revisionEdit.setText(game.revision);
  boardEdit.setText(game.board);
  nameEdit.setText(game.name);
  labelEdit.setText(game.label);
  noteEdit.setText(game.note);
  acceptButton.setText(create ? "Create" : "Modify");

  reloadList();
  updateWindow();
  setCentered(*listWindow);
  setVisible();

  if(create) {
    hashEdit.setFocused();
  } else {
    cancelButton.setFocused();
  }
}

auto GameWindow::accept() -> void {
  game.sha256 = hashEdit.text().strip();
  game.region = regionEdit.text().strip();
  game.revision = revisionEdit.text().strip();
  game.board = boardEdit.text().strip();
  game.name = nameEdit.text().strip();
  game.label = labelEdit.text().strip();
  game.note = noteEdit.text().strip();

  if(create) {
    listWindow->appendGame(game);
  } else {
    listWindow->modifyGame(game);
  }

  memoryWindow->setVisible(false);
  setVisible(false);
  listWindow->setEnabled();
  listWindow->setFocused();
}

auto GameWindow::cancel() -> void {
  if(!modified || MessageDialog().setParent(*this).setText({
    "Are you sure you want to discard your changes to this game?"
  }).question() == "Yes") {
    memoryWindow->setVisible(false);
    setVisible(false);
    listWindow->setEnabled();
    listWindow->setFocused();
  }
}

auto GameWindow::reloadList() -> void {
  memoryList.reset();
  memoryList.append(TableViewHeader()
    .append(TableViewColumn().setText("Type"))
    .append(TableViewColumn().setText("Size"))
    .append(TableViewColumn().setText("Name").setExpandable())
  );
  for(auto& memory : game.memories) {
    memoryList.append(TableViewItem()
      .append(TableViewCell().setText(memory.type))
      .append(TableViewCell().setText(memory.size))
      .append(TableViewCell().setText(memory.name))
    );
  }
  Application::processEvents();
  memoryList.resizeColumns();
}

auto GameWindow::updateWindow() -> void {
  bool valid = true;
  hashEdit.setBackgroundColor(
    !create ? Color{192, 255, 192}
  : hashEdit.text().strip().size() == 64 ? Color{}
  : (valid = false, Color{255, 224, 224})
  );
  regionEdit.setBackgroundColor(regionEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  revisionEdit.setBackgroundColor(revisionEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  boardEdit.setBackgroundColor(boardEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  nameEdit.setBackgroundColor(nameEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  labelEdit.setBackgroundColor(labelEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  noteEdit.setBackgroundColor(noteEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  modifyButton.setEnabled((bool)memoryList.selected());
  removeButton.setEnabled((bool)memoryList.selected());
  acceptButton.setEnabled(valid);
  setTitle({modified ? "*" : "", create ? "Add New Game" : "Modify Game Details"});
}

auto GameWindow::appendMemory(Memory memory) -> void {
  modified = true;
  auto offset = game.memories.size();
  game.memories.append(memory);
  reloadList();
  memoryList.item(offset).setSelected().setFocused();
  updateWindow();
}

auto GameWindow::modifyMemory(Memory memory) -> void {
  if(auto item = memoryList.selected()) {
    modified = true;
    auto offset = item.offset();
    game.memories[offset] = memory;
    reloadList();
    memoryList.item(offset).setSelected().setFocused();
    updateWindow();
  }
}

auto GameWindow::removeMemory() -> void {
  if(auto item = memoryList.selected()) {
    if(MessageDialog().setParent(*this).setText({
      "Are you sure you want to permanently remove this memory?\n\n",
      "Name: ", item.cell(2).text()
    }).question() == "Yes") {
      modified = true;
      game.memories.remove(item.offset());
      reloadList();
      updateWindow();
    }
  }
}

//

MemoryWindow::MemoryWindow() {
  memoryWindow = this;

  layout.setMargin(5);
  typeLabel.setText("Type:");
  typeEdit.append(ComboEditItem().setText("ROM"));
  typeEdit.append(ComboEditItem().setText("EPROM"));
  typeEdit.append(ComboEditItem().setText("EEPROM"));
  typeEdit.append(ComboEditItem().setText("NOR"));
  typeEdit.append(ComboEditItem().setText("PSRAM"));
  typeEdit.append(ComboEditItem().setText("NVRAM"));
  typeEdit.append(ComboEditItem().setText("RAM"));
  typeEdit.onChange([&] { modified = true, updateWindow(); });
  sizeLabel.setText("Size:");
  sizeEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  nameLabel.setText("Name:");
  nameEdit.onChange([&] { modified = true, updateWindow(); });
  acceptButton.setText("Accept").onActivate([&] { accept(); });
  cancelButton.setText("Cancel").onActivate([&] { cancel(); });

  onClose([&] { cancel(); });

  setSize({280, layout.minimumSize().height()});
  setDismissable();
}

auto MemoryWindow::show(Memory memory) -> void {
  this->memory = memory;
  modified = false;
  create = !memory.type;

  typeEdit.setText(memory.type);
  sizeEdit.setText(memory.size);
  nameEdit.setText(memory.name);

  updateWindow();
  setCentered(*gameWindow);
  setVisible();

  typeEdit.setFocused();
}

auto MemoryWindow::accept() -> void {
  memory.type = typeEdit.text().strip();
  memory.size = sizeEdit.text().strip();
  memory.name = nameEdit.text().strip();

  if(create) {
    gameWindow->appendMemory(memory);
  } else {
    gameWindow->modifyMemory(memory);
  }

  setVisible(false);
  gameWindow->setEnabled();
  gameWindow->setFocused();
}

auto MemoryWindow::cancel() -> void {
  if(!modified || MessageDialog().setParent(*this).setText({
    "Are you sure you want to discard your changes to this memory?"
  }).question() == "Yes") {
    setVisible(false);
    gameWindow->setEnabled();
    gameWindow->setFocused();
  }
}

auto MemoryWindow::updateWindow() -> void {
  bool valid = true;
  typeEdit.setBackgroundColor(typeEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  sizeEdit.setBackgroundColor(sizeEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  nameEdit.setBackgroundColor(nameEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  acceptButton.setEnabled(valid);
  setTitle({modified ? "*" : "", create ? "Add New Memory" : "Modify Memory Details"});
}

//

#include <nall/main.hpp>
auto nall::main(string_vector) -> void {
  Application::setName("genius");
  new ListWindow;
  new GameWindow;
  new MemoryWindow;
  Application::run();
}
