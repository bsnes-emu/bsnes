string Utility::selectCartridge(unsigned cartridgeType) {
  string match;

  if(cartridgeType == AnyCartridge) {
    match = "All cartridges (*.sfc *.smc *.swc *.fig *.bs *.st *.gb *.gbc #GZIP #JMA);;"
            "SNES cartridges (*.sfc *.smc *.swc *.fig *.bs *.st #GZIP #JMA);;"
            "Gameboy cartridges (*.gb *.gbc #GZIP #JMA);;";
  } else if(cartridgeType == SnesCartridge) {
    match = "SNES cartridges (*.sfc *.smc *.swc *.fig *.bs *.st #GZIP #JMA);;";
  } else if(cartridgeType == GameboyCartridge) {
    match = "Gameboy cartridges (*.gb *.gbc #GZIP #JMA);;";
  } else return "";

  #if defined(GZIP_SUPPORT)
  match.replace(" #GZIP", " *.zip *.gz");
  #else
  match.replace(" #GZIP", "");
  #endif

  #if defined(JMA_SUPPORT)
  match.replace(" #JMA", " *.jma");
  #else
  match.replace( "#JMA", "");
  #endif

  match << "All files (*)";

  audio.clear();
  QString filename = QFileDialog::getOpenFileName(0, "Load Cartridge",
    utf8() << (config.path.rom != "" ? config.path.rom : config.path.current),
    utf8() << match
  );
  return string() << filename.toUtf8().constData();
}

string Utility::selectFolder(const char *title) {
  audio.clear();
  QString pathname = QFileDialog::getExistingDirectory(0,
    title, utf8() << config.path.current,
    QFileDialog::ShowDirsOnly);
  return string() << pathname.toUtf8().constData();
}

void Utility::loadCartridge(const char *filename) {
  switch(cartridge.detectImageType(filename)) {
    case SNES::Cartridge::TypeNormal:           cartridge.loadNormal(filename);                                    break;
    case SNES::Cartridge::TypeBsxSlotted:       winLoader->loadBsxSlottedCartridge(filename, "");                  break;
    case SNES::Cartridge::TypeBsxBios:          winLoader->loadBsxCartridge(filename, "");                         break;
    case SNES::Cartridge::TypeBsx:              winLoader->loadBsxCartridge(config.path.bsx, filename);            break;
    case SNES::Cartridge::TypeSufamiTurboBios:  winLoader->loadSufamiTurboCartridge(filename, "", "");             break;
    case SNES::Cartridge::TypeSufamiTurbo:      winLoader->loadSufamiTurboCartridge(config.path.st, filename, ""); break;
    case SNES::Cartridge::TypeSuperGameboyBios: winLoader->loadSuperGameboyCartridge(filename, "");                break;
    case SNES::Cartridge::TypeGameboy:          winLoader->loadSuperGameboyCartridge(config.path.sgb, filename);   break;
  }
}

void Utility::modifySystemState(system_state_t state) {
  video.clear();
  audio.clear();

  switch(state) {
    case LoadCartridge: {
      //must call cartridge.load_cart_...() before calling modifySystemState(LoadCartridge)
      if(SNES::cartridge.loaded() == false) break;
      config.path.current = Cartridge::basepath(cartridge.cartName);

      application.power = true;
      application.pause = false;
      SNES::system.power();

      //warn if unsupported hardware detected
      string chip;
      if(SNES::cartridge.has_superfx())    chip = "SuperFX";
      else if(SNES::cartridge.has_st011()) chip = "ST011";
      else if(SNES::cartridge.has_st018()) chip = "ST018";
      else if(SNES::cartridge.has_dsp3())  chip = "DSP-3";  //unplayable (only partially supported)
      if(chip != "") {
        QMessageBox::warning(winMain->window, "Warning", utf8()
        << "<p><b>Warning:</b><br>Unsupported " << chip << " chip detected. "
        << "It is unlikely that this title will work properly.</p>");
      }

      showMessage(utf8()
      << "Loaded " << cartridge.name
      << (SNES::cartridge.patched() ? ", and applied UPS patch." : "."));
      winMain->window->setWindowTitle(utf8() << BSNES_TITLE << " - " << cartridge.name);
    } break;

    case UnloadCartridge: {
      if(SNES::cartridge.loaded() == false) break;  //no cart to unload?
      SNES::cartridge.unload();

      application.power = false;
      application.pause = true;

      showMessage(utf8() << "Unloaded " << cartridge.name << ".");
      winMain->window->setWindowTitle(utf8() << BSNES_TITLE);
    } break;

    case PowerOn: {
      if(SNES::cartridge.loaded() == false || application.power == true) break;

      application.power = true;
      application.pause = false;
      SNES::system.power();

      showMessage("Power on.");
    } break;

    case PowerOff: {
      if(SNES::cartridge.loaded() == false || application.power == false) break;

      application.power = false;
      application.pause = true;

      showMessage("Power off.");
    } break;

    case PowerCycle: {
      if(SNES::cartridge.loaded() == false) break;

      application.power = true;
      application.pause = false;
      SNES::system.power();

      showMessage("System power was cycled.");
    } break;

    case Reset: {
      if(SNES::cartridge.loaded() == false || application.power == false) break;

      application.pause = false;
      SNES::system.reset();

      showMessage("System was reset.");
    } break;
  }

  winMain->syncUi();
  winCodeEditor->dismiss();
  winCheatEditor->reloadList();
  winCheatEditor->syncUi();
}
