DipSwitches *dipSwitches = nullptr;

DipSwitch::DipSwitch() {
  append(name,  {~0, 0}, 5);
  append(value, {~0, 0}, 0);
}

DipSwitches::DipSwitches() {
  setTitle("DIP Switches");

  layout.setMargin(5);
  acceptButton.setText("Accept");

  append(layout);
  for(unsigned n = 0; n < 8; n++)
  layout.append(dip[n],                {~0, 0}, 5);
  layout.append(controlLayout,         {~0, 0}, 5);
    controlLayout.append(spacer,       {~0, 0}, 0);
    controlLayout.append(acceptButton, { 0, 0}, 0);

  setGeometry({128, 128, 400, layout.minimumGeometry().height});
  windowManager->append(this, "DipSwitches");

  acceptButton.onActivate = { &DipSwitches::accept, this };
}

void DipSwitches::load() {
  if(interface->mode() != Interface::Mode::SFC || SFC::cartridge.has_nss_dip() == false) return;
  application->pause = true;

  auto info = SFC::cartridge.information.nss;
  unsigned count = info.setting.size();

  for(unsigned n = 0; n < min(8, count); n++) {
    dip[n].setEnabled(true);
    dip[n].name.setText(info.setting[n]);
    dip[n].value.reset();
    for(unsigned z = 0; z < min(16, info.option[n].size()); z++) {
      lstring part;
      part.split<1>(":", info.option[n][z]);
      values[n][z] = hex(part[0]);
      dip[n].value.append(part[1]);
    }
  }

  for(unsigned n = count; n < 8; n++) {
    dip[n].setEnabled(false);
    dip[n].name.setText("(unused)");
    dip[n].value.reset();
    dip[n].value.append("(unused)");
  }

  acceptButton.setFocused();
  setVisible();
}

void DipSwitches::accept() {
  auto info = SFC::cartridge.information.nss;
  unsigned count = info.setting.size();

  unsigned result = 0x0000;
  for(unsigned n = 0; n < min(8, count); n++) {
    result |= values[n][dip[n].value.selection()];
  }

  setVisible(false);
  SFC::nss.set_dip(result);
  application->pause = false;
}
