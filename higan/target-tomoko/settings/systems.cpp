SystemSettings::SystemSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Device::Storage);
  setText("Systems");

  layout.setMargin(5);

  systemList.onActivate([&] {
    modifyButton.doActivate();
  });

  systemList.onChange([&] {
    auto selected = (bool)systemList.selected();
    upButton.setEnabled(selected);
    downButton.setEnabled(selected);
    modifyButton.setEnabled(selected);
    removeButton.setEnabled(selected);
  });

  upButton.setIcon(Icon::Go::Up);
  downButton.setIcon(Icon::Go::Down);

  appendButton.setText("Append").onActivate([&] {
    systemProperties->append();
  });

  modifyButton.setText("Modify").onActivate([&] {
    if(auto item = systemList.selected()) {
      if(auto system = settings.find("Systems/System")[item.offset()]) {
        systemProperties->modify(system);
      }
    }
  });

  removeButton.setText("Remove").onActivate([&] {
    if(auto item = systemList.selected()) {
      if(auto system = settings.find("Systems/System")[item.offset()]) {
        if(MessageDialog().setParent(*settingsManager).setText({
          "Are you sure you want to delete this system?\n\n"
          "Name: ", system["Name"].text()
        }).question() == "Yes") {
          settings["Systems"].remove(system);
          reload();
        }
      }
    }
  });

  reload();
}

auto SystemSettings::reload() -> void {
  systemList.reset();
  systemList.append(TableViewHeader().setVisible()
    .append(TableViewColumn().setText("Name"))
    .append(TableViewColumn().setText("Boot").setExpandable())
  );
  for(auto system : settings.find("Systems/System")) {
    systemList.append(TableViewItem()
      .append(TableViewCell().setText(system["Name"].text()))
      .append(TableViewCell().setText(Location::base(system["Boot"].text()).trimRight("/", 1L)))
      .setForegroundColor(system["Hidden"].boolean() ? Color{160, 160, 160} : Color{})
    );
  }
  systemList.resizeColumns().doChange();
  presentation->loadSystems();
}

auto SystemSettings::acceptProperties() -> void {
  if(systemProperties->acceptButton.text() == "Append") {
    Markup::Node system{"System"};
    system.append({"Name", systemProperties->nameEdit.text()});
    system.append({"Boot", systemProperties->bootEdit.text()});
    system.append({"Hidden", systemProperties->hiddenOption.checked()});
    settings["Systems"].append(system);
  } else if(systemProperties->acceptButton.text() == "Modify") {
    if(auto item = systemList.selected()) {
      if(auto system = settings.find("Systems/System")[item.offset()]) {
        system("Name").setValue(systemProperties->nameEdit.text());
        system("Boot").setValue(systemProperties->bootEdit.text());
        system("Hidden").setValue(systemProperties->hiddenOption.checked());
      }
    }
  }
  reload();
}
