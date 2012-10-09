DipSwitches *dipSwitches = nullptr;

DipSwitch::DipSwitch() {
  append(name, {100, 0}, 5);
  append(value, {~0, 0});
}

DipSwitches::DipSwitches() {
  setTitle("DIP Switches");
  layout.setMargin(5);
  accept.setText("Accept");

  append(layout);
  for(auto &dipItem : dip) layout.append(dipItem, {~0, 0}, 5);
  layout.append(controlLayout, {~0, 0});
    controlLayout.append(spacer, {~0, 0});
    controlLayout.append(accept, {80, 0});

  setGeometry({128, 128, 250, layout.minimumGeometry().height});

  onClose = accept.onActivate = [&] { quit = true; };
}

unsigned DipSwitches::run(const Markup::Node &node) {
  audio.clear();
  setModal(true);
  quit = false;

  for(auto &dipItem : dip) {
    dipItem.name.setEnabled(false);
    dipItem.name.setText("(empty)");
    dipItem.value.setEnabled(false);
    dipItem.value.reset();
    dipItem.values.reset();
  }

  unsigned index = 0;
  for(auto &setting : node) {
    if(setting.name != "setting") continue;
    dip[index].name.setEnabled();
    dip[index].name.setText(setting["name"].data);
    dip[index].value.setEnabled();
    for(auto &option : setting) {
      if(option.name != "option") continue;
      dip[index].value.append(option["name"].data);
      dip[index].values.append(fixedpoint::parse(option["value"].data));
    }

    if(++index >= Slots) break;
  }

  setVisible();
  accept.setFocused();

  while(quit == false) {
    OS::processEvents();
  }

  setModal(false);
  setVisible(false);

  unsigned result = 0;
  for(auto &dipItem : dip) {
    if(dipItem.value.enabled() == false) continue;
    result |= dipItem.values[dipItem.value.selection()];
  }
  return result;
}
