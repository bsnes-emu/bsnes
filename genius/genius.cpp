#include <nall/nall.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

#include "genius.hpp"
unique_pointer<ListWindow> listWindow;
unique_pointer<GameWindow> gameWindow;
unique_pointer<MemoryWindow> memoryWindow;
unique_pointer<OscillatorWindow> oscillatorWindow;

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
      "genius\n",
      "Author: byuu\n",
      "Website: https://byuu.org/"
    }).information();
  });

  layout.setPadding(5);
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

  setSize({820, 600});
  reloadList();
  updateWindow();
  setCentered();
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
    game.label = node["label"].text();
    game.name = node["name"].text();
    game.region = node["region"].text();
    game.revision = node["revision"].text();
    game.board = node["board"].text();
    for(auto object : node["board"]) {
      Component component;
      if(object.name() == "memory") {
        component.type = Component::Type::Memory;
        component.memory.type = object["type"].text();
        component.memory.size = object["size"].text();
        component.memory.content = object["content"].text();
        component.memory.manufacturer = object["manufacturer"].text();
        component.memory.architecture = object["architecture"].text();
        component.memory.identifier = object["identifier"].text();
        component.memory.Volatile = (bool)object["volatile"];
      }
      if(object.name() == "oscillator") {
        component.type = Component::Type::Oscillator;
        component.oscillator.frequency = object["frequency"].text();
      }
      game.components.append(component);
    }
    game.note = node["note"].text();
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
    return string::icompare(
      {x.name, "\n", x.region, "\n", x.revision},
      {y.name, "\n", y.region, "\n", y.revision}
    ) < 0;
  });

  fp.print("database\n");
  fp.print("  revision: ", chrono::local::date(), "\n\n");

  for(auto& game : copy) {
    fp.print("game\n");
    fp.print("  sha256:   ", game.sha256, "\n");
  if(game.label)
    fp.print("  label:    ", game.label, "\n");
    fp.print("  name:     ", game.name, "\n");
    fp.print("  region:   ", game.region, "\n");
    fp.print("  revision: ", game.revision, "\n");
  if(game.board)
    fp.print("  board:    ", game.board, "\n");
  else if(game.components)
    fp.print("  board\n");
    for(auto& component : game.components) {
      if(component.type == Component::Type::Memory) {
        fp.print("    memory\n");
        fp.print("      type: ", component.memory.type, "\n");
        fp.print("      size: ", component.memory.size, "\n");
        fp.print("      content: ", component.memory.content, "\n");
      if(component.memory.manufacturer)
        fp.print("      manufacturer: ", component.memory.manufacturer, "\n");
      if(component.memory.architecture)
        fp.print("      architecture: ", component.memory.architecture, "\n");
      if(component.memory.identifier)
        fp.print("      identifier: ", component.memory.identifier, "\n");
      if(component.memory.Volatile)
        fp.print("      volatile\n");
      }

      if(component.type == Component::Type::Oscillator) {
        fp.print("    oscillator\n");
        fp.print("      frequency: ", component.oscillator.frequency, "\n");
      }
    }
  if(game.note)
    fp.print("  note: ", game.note, "\n");
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

  layout.setPadding(5);
  hashLabel.setText("SHA256:").setAlignment(1.0);
  hashEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  regionLabel.setText("Region:").setAlignment(1.0);
  regionEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  revisionLabel.setText("Revision:");
  revisionEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  boardLabel.setText("Board:");
  boardEdit.setFont(Font().setFamily(Font::Mono)).onChange([&] { modified = true, updateWindow(); });
  nameLabel.setText("Name:").setAlignment(1.0);
  nameEdit.onChange([&] { modified = true, updateWindow(); });
  labelLabel.setText("Label:").setAlignment(1.0);
  labelEdit.onChange([&] { modified = true, updateWindow(); });
  noteLabel.setText("Note:").setAlignment(1.0);
  noteEdit.onChange([&] { modified = true, updateWindow(); });
  componentLabel.setText("Tree:").setAlignment({1.0, 0.0});
  componentTree.onActivate([&] { modifyComponentButton.doActivate(); });
  componentTree.onChange([&] { updateWindow(); });
  appendMemoryButton.setText("Memory").onActivate([&] {
    setEnabled(false);
    memoryWindow->show();
  });
  appendOscillatorButton.setText("Oscillator").onActivate([&] {
    setEnabled(false);
    oscillatorWindow->show();
  });
  modifyComponentButton.setText("Modify").onActivate([&] {
    if(auto item = componentTree.selected()) {
      setEnabled(false);
      auto path = item.path().split("/");
      auto offset = path(0).natural();
      Component component = game.components[offset];
      if(component.type == Component::Type::Memory) {
        memoryWindow->show(component.memory);
      }
      if(component.type == Component::Type::Oscillator) {
        oscillatorWindow->show(component.oscillator);
      }
    }
  });
  removeComponentButton.setText("Remove").onActivate([&] { removeComponent(); });
  acceptButton.setText("Accept").onActivate([&] { accept(); });
  cancelButton.setText("Cancel").onActivate([&] { cancel(); });

  onClose([&] { cancel(); });

  setSize({640, 480});
  setDismissable();
}

auto GameWindow::show(Game game) -> void {
  this->game = game;
  modified = false;
  create = !game.sha256;

  hashEdit.setText(game.sha256);
  regionEdit.setText(game.region);
  revisionEdit.setText(game.revision);
  boardEdit.setText(game.board);
  nameEdit.setText(game.name);
  labelEdit.setText(game.label);
  noteEdit.setText(game.note);
  acceptButton.setText(create ? "Create" : "Apply");

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
  componentTree.reset();
  uint counter = 1;
  for(auto& component : game.components) {
    TreeViewItem item;

    string index = {"[", counter++, "] "};
    if(component.type == Component::Type::Memory) {
      item.setText({index, "Memory"});
      item.append(TreeViewItem().setText({"Type: ", component.memory.type}));
      item.append(TreeViewItem().setText({"Size: ", component.memory.size}));
      item.append(TreeViewItem().setText({"Content: ", component.memory.content}));
    if(component.memory.manufacturer)
      item.append(TreeViewItem().setText({"Manufacturer: ", component.memory.manufacturer}));
    if(component.memory.architecture)
      item.append(TreeViewItem().setText({"Architecture: ", component.memory.architecture}));
    if(component.memory.identifier)
      item.append(TreeViewItem().setText({"Identifier: ", component.memory.identifier}));
    if(component.memory.Volatile)
      item.append(TreeViewItem().setText({"Volatile"}));
    }

    if(component.type == Component::Type::Oscillator) {
      item.setText({index, "Oscillator"});
      item.append(TreeViewItem().setText({"Frequency: ", component.oscillator.frequency}));
    }

    componentTree.append(item);
  }

  Application::processEvents();
  for(auto& item : componentTree.items()) item.setExpanded();
}

auto GameWindow::updateWindow() -> void {
  bool valid = true;
  bool hashValid = hashEdit.text().strip().size() == 64;
  hashEdit.setEditable(!hashValid).setBackgroundColor(
    !create || hashValid ? Color{192, 255, 192}
  : (valid = false, Color{255, 224, 224}));
  regionEdit.setBackgroundColor(regionEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  revisionEdit.setBackgroundColor(revisionEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  boardEdit.setBackgroundColor(boardEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  nameEdit.setBackgroundColor(nameEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  labelEdit.setBackgroundColor(labelEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  noteEdit.setBackgroundColor(noteEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  modifyComponentButton.setEnabled((bool)componentTree.selected());
  removeComponentButton.setEnabled((bool)componentTree.selected());
  acceptButton.setEnabled(valid);
  setTitle({modified ? "*" : "", create ? "Add New Game" : "Modify Game Details"});
  if(create && hashValid && hashEdit.focused()) regionEdit.setFocused();
}

auto GameWindow::appendComponent(Component component) -> void {
  modified = true;
  auto offset = game.components.size();
  game.components.append(component);
  reloadList();
  componentTree.item(offset).setSelected().setFocused();
  updateWindow();
}

auto GameWindow::modifyComponent(Component component) -> void {
  if(auto item = componentTree.selected()) {
    modified = true;
    auto path = item.path().split("/");
    auto offset = path(0).natural();
    game.components[offset] = component;
    reloadList();
    componentTree.item(offset).setSelected().setFocused();
    updateWindow();
  }
}

auto GameWindow::removeComponent() -> void {
  if(auto item = componentTree.selected()) {
    if(MessageDialog().setParent(*this).setText({
      "Are you sure you want to permanently remove this component?"
    }).question() == "Yes") {
      modified = true;
      auto path = item.path().split("/");
      auto offset = path(0).natural();
      game.components.remove(offset);
      reloadList();
      updateWindow();
    }
  }
}

//

MemoryWindow::MemoryWindow() {
  memoryWindow = this;

  layout.setPadding(5);
  typeLabel.setText("Type:").setAlignment(1.0);
  typeEdit.append(ComboEditItem().setText("ROM"));
  typeEdit.append(ComboEditItem().setText("EEPROM"));
  typeEdit.append(ComboEditItem().setText("Flash"));
  typeEdit.append(ComboEditItem().setText("RAM"));
  typeEdit.append(ComboEditItem().setText("RTC"));
  typeEdit.onChange([&] { modified = true, updateWindow(); });
  sizeLabel.setText("Size:").setAlignment(1.0);
  sizeEdit.onChange([&] { modified = true, updateWindow(); });
  contentLabel.setText("Content:").setAlignment(1.0);
  contentEdit.append(ComboEditItem().setText("Program"));
  contentEdit.append(ComboEditItem().setText("Data"));
  contentEdit.append(ComboEditItem().setText("Character"));
  contentEdit.append(ComboEditItem().setText("Save"));
  contentEdit.append(ComboEditItem().setText("Time"));
  contentEdit.onChange([&] { modified = true, updateWindow(); });
  manufacturerLabel.setText("Manufacturer:").setAlignment(1.0);
  manufacturerEdit.onChange([&] { modified = true, updateWindow(); });
  architectureLabel.setText("Architecture:").setAlignment(1.0);
  architectureEdit.onChange([&] { modified = true, updateWindow(); });
  identifierLabel.setText("Identifier:").setAlignment(1.0);
  identifierEdit.onChange([&] { modified = true, updateWindow(); });
  volatileOption.setText("Volatile").onToggle([&] { modified = true, updateWindow(); });
  acceptButton.setText("Accept").onActivate([&] { accept(); });
  cancelButton.setText("Cancel").onActivate([&] { cancel(); });

  onClose([&] { cancel(); });

  setSize({320, layout.minimumSize().height()});
  setDismissable();
}

auto MemoryWindow::show(Memory memory) -> void {
  this->memory = memory;
  modified = false;
  create = !memory.type;

  typeEdit.setText(memory.type);
  sizeEdit.setText(memory.size);
  contentEdit.setText(memory.content);
  manufacturerEdit.setText(memory.manufacturer);
  architectureEdit.setText(memory.architecture);
  identifierEdit.setText(memory.identifier);
  volatileOption.setChecked(memory.Volatile);

  updateWindow();
  setCentered(*gameWindow);
  setVisible();

  typeEdit.setFocused();
}

auto MemoryWindow::accept() -> void {
  memory.type = typeEdit.text().strip();
  memory.size = sizeEdit.text().strip();
  memory.content = contentEdit.text().strip();
  memory.manufacturer = manufacturerEdit.text().strip();
  memory.architecture = architectureEdit.text().strip();
  memory.identifier = identifierEdit.text().strip();
  memory.Volatile = volatileOption.checked() && (memory.type == "RAM" || memory.type == "RTC");

  Component component{Component::Type::Memory};
  component.memory = memory;
  if(create) {
    gameWindow->appendComponent(component);
  } else {
    gameWindow->modifyComponent(component);
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
  contentEdit.setBackgroundColor(contentEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  manufacturerEdit.setBackgroundColor(manufacturerEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  architectureEdit.setBackgroundColor(architectureEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  identifierEdit.setBackgroundColor(identifierEdit.text().strip() ? Color{} : (Color{255, 255, 240}));
  volatileOption.setEnabled(typeEdit.text().strip() == "RAM" || typeEdit.text().strip() == "RTC");
  acceptButton.setEnabled(valid);
  setTitle({modified ? "*" : "", create ? "Add New Memory" : "Modify Memory Details"});
}

//

OscillatorWindow::OscillatorWindow() {
  oscillatorWindow = this;

  layout.setPadding(5);
  frequencyLabel.setText("Frequency:").setAlignment(1.0);
  frequencyEdit.onChange([&] { modified = true, updateWindow(); });
  acceptButton.setText("Accept").onActivate([&] { accept(); });
  cancelButton.setText("Cancel").onActivate([&] { cancel(); });

  onClose([&] { cancel(); });

  setSize({320, layout.minimumSize().height()});
  setDismissable();
}

auto OscillatorWindow::show(Oscillator oscillator) -> void {
  this->oscillator = oscillator;
  modified = false;
  create = !oscillator.frequency;

  frequencyEdit.setText(oscillator.frequency);

  updateWindow();
  setCentered(*gameWindow);
  setVisible();

  frequencyEdit.setFocused();
}

auto OscillatorWindow::accept() -> void {
  oscillator.frequency = frequencyEdit.text().strip();

  Component component{Component::Type::Oscillator};
  component.oscillator = oscillator;
  if(create) {
    gameWindow->appendComponent(component);
  } else {
    gameWindow->modifyComponent(component);
  }

  setVisible(false);
  gameWindow->setEnabled();
  gameWindow->setFocused();
}

auto OscillatorWindow::cancel() -> void {
  if(!modified || MessageDialog().setParent(*this).setText({
    "Are you sure you want to discard your changes to this property?"
  }).question() == "Yes") {
    setVisible(false);
    gameWindow->setEnabled();
    gameWindow->setFocused();
  }
}

auto OscillatorWindow::updateWindow() -> void {
  bool valid = true;
  frequencyEdit.setBackgroundColor(frequencyEdit.text().strip() ? Color{} : (valid = false, Color{255, 224, 224}));
  acceptButton.setEnabled(valid);
  setTitle({modified ? "*" : "", create ? "Add New Property" : "Modify Property Details"});
}

//

#include <nall/main.hpp>
auto nall::main(string_vector args) -> void {
  Application::setName("genius");
  new ListWindow;
  new GameWindow;
  new MemoryWindow;
  new OscillatorWindow;

  //internal command used to synchronize all genius databases from an old format to a new format
  //if enabled, use with extreme caution and make backups first
/*if(args.size() == 3 && args[1] == "--sync") {
    for(auto& filename : directory::contents(args[2], "*.bml")) {
      if(filename.beginsWith("Boards")) continue;
      print(filename, "\n");
      listWindow->loadDatabase({args[2], filename});
      listWindow->saveDatabase({args[2], filename});
    }
    return print("[Done]\n");
  }*/

  listWindow->setVisible();
  Application::run();
}
