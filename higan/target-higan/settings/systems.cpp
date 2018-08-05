SystemSettings::SystemSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Storage);
  setText("Systems");

  layout.setPadding(5);

  systemList.onActivate([&] {
    modifyButton.doActivate();
  });

  systemList.onChange([&] {
    auto selected = systemList.selected();
    upButton.setEnabled((bool)selected && selected.offset() != 0);
    downButton.setEnabled((bool)selected && selected.offset() != systemList.itemCount() - 1);
    modifyButton.setEnabled((bool)selected);
    removeButton.setEnabled((bool)selected);
  });

  systemList.onToggle([&](TableViewCell cell) {
    toggle(cell);
  });

  upButton.setIcon(Icon::Go::Up).onActivate([&] { moveUp(); });
  downButton.setIcon(Icon::Go::Down).onActivate([&] { moveDown(); });

  appendButton.setText("Append").onActivate([&] { systemProperties->append(); });
  modifyButton.setText("Modify").onActivate([&] { modify(); });
  removeButton.setText("Remove").onActivate([&] { remove(); });

  reload();
}

auto SystemSettings::reload() -> void {
  systemList.reset();
  systemList.append(TableViewColumn());
  systemList.append(TableViewColumn().setText("System").setExpandable());
  for(auto system : settings.find("Systems/System")) {
    string name = system.text();
    string load = Location::base(system["Load"].text()).trimRight("/", 1L);
    string alias = system["Alias"].text();
    systemList.append(TableViewItem()
      .append(TableViewCell()
        .setCheckable()
        .setChecked(system["Visible"].boolean())
      )
      .append(TableViewCell()
        .setIcon(load ? (image)Icon::Emblem::Folder : (image)Icon::Device::Storage)
        .setText(alias ? alias : load ? load : name)
      )
    );
  }
  systemList.resizeColumns().doChange();
  presentation->loadSystems();
}

auto SystemSettings::toggle(TableViewCell cell) -> void {
  if(auto item = cell->parentTableViewItem()) {
    if(auto system = settings.find("Systems/System")[item->offset()]) {
      system("Visible").setValue(item->cell(0).checked());
      presentation->loadSystems();
    }
  }
}

auto SystemSettings::moveUp() -> void {
  if(auto item = systemList.selected()) {
    auto offset = item.offset();
    settings["Systems"].swap(offset, offset - 1);
    reload();
    systemList.item(offset - 1).setSelected();
    systemList.doChange();
  }
}

auto SystemSettings::moveDown() -> void {
  if(auto item = systemList.selected()) {
    auto offset = item.offset();
    settings["Systems"].swap(offset, offset + 1);
    reload();
    systemList.item(offset + 1).setSelected();
    systemList.doChange();
  }
}

auto SystemSettings::modify() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = settings.find("Systems/System")[item.offset()]) {
      systemProperties->modify(system);
    }
  }
}

auto SystemSettings::remove() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = settings.find("Systems/System")[item.offset()]) {
      if(MessageDialog().setParent(*settingsManager).setText({
        "Are you sure you want to delete this system?\n\n",
        item.cell(1).text()
      }).question() == "Yes") {
        settings["Systems"].remove(system);
        reload();
      }
    }
  }
}

auto SystemSettings::accept() -> void {
  if(systemProperties->acceptButton.text() == "Append") {
    Markup::Node system{"System", systemProperties->systemOption.selected().text()};
    system.append({"Load", systemProperties->loadEdit.text()});
    system.append({"Alias", systemProperties->aliasEdit.text()});
    system.append({"Visible", "true"});
    settings["Systems"].append(system);
  } else if(systemProperties->acceptButton.text() == "Modify") {
    if(auto item = systemList.selected()) {
      if(auto system = settings.find("Systems/System")[item.offset()]) {
        system.setValue(systemProperties->systemOption.selected().text());
        system("Load").setValue(systemProperties->loadEdit.text());
        system("Alias").setValue(systemProperties->aliasEdit.text());
      }
    }
  }
  reload();
}
