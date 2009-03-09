string Utility::selectCartridge() {
  audio.clear();
  QString filename = QFileDialog::getOpenFileName(0,
    "Load Cartridge",
    utf8() << (snes.config.path.rom != "" ? snes.config.path.rom : snes.config.path.current),
    "SNES images (*.smc *.sfc *.swc *.fig *.bs *.st"
    #if defined(GZIP_SUPPORT)
    " *.zip *.gz"
    #endif
    #if defined(JMA_SUPPORT)
    " *.jma"
    #endif
    ");;"
    "All files (*)"
  );
  return string() << filename.toUtf8().constData();
}

string Utility::selectFolder(const char *title) {
  audio.clear();
  QString pathname = QFileDialog::getExistingDirectory(0,
    title, utf8() << snes.config.path.current,
    QFileDialog::ShowDirsOnly);
  return string() << pathname.toUtf8().constData();
}

void Utility::loadCartridge(const char *filename) {
  switch(cartridge.detect_image_type(filename)) {
    case Cartridge::TypeNormal:          loadCartridgeNormal(filename);                                          break;
    case Cartridge::TypeBsxSlotted:      winLoader->loadBsxSlottedCartridge(filename, "");                       break;
    case Cartridge::TypeBsxBios:         winLoader->loadBsxCartridge(filename, "");                              break;
    case Cartridge::TypeBsx:             winLoader->loadBsxCartridge(snes.config.path.bsx, filename);            break;
    case Cartridge::TypeSufamiTurboBios: winLoader->loadSufamiTurboCartridge(filename, "", "");                  break;
    case Cartridge::TypeSufamiTurbo:     winLoader->loadSufamiTurboCartridge(snes.config.path.st, filename, ""); break;
  }
}

bool Utility::loadCartridgeNormal(const char *base) {
  if(!*base) return false;
  unloadCartridge();
  cartridge.load_normal(base);
  modifySystemState(LoadCartridge);
  return true;
}

bool Utility::loadCartridgeBsxSlotted(const char *base, const char *slot) {
  if(!*base) return false;
  unloadCartridge();
  cartridge.load_bsx_slotted(base, slot);
  modifySystemState(LoadCartridge);
  return true;
}

bool Utility::loadCartridgeBsx(const char *base, const char *slot) {
  if(!*base) return false;
  unloadCartridge();
  cartridge.load_bsx(base, slot);
  modifySystemState(LoadCartridge);
  return true;
}

bool Utility::loadCartridgeSufamiTurbo(const char *base, const char *slotA, const char *slotB) {
  if(!*base) return false;
  unloadCartridge();
  cartridge.load_sufami_turbo(base, slotA, slotB);
  modifySystemState(LoadCartridge);
  return true;
}

void Utility::unloadCartridge() {
  if(cartridge.loaded()) {
    cartridge.unload();
    modifySystemState(UnloadCartridge);
  }
}

void Utility::modifySystemState(system_state_t state) {
  video.clear();
  audio.clear();

  switch(state) {
    case LoadCartridge: {
      //must call cartridge.load_cart_...() before calling modifySystemState(LoadCartridge)
      if(cartridge.loaded() == false) break;

      application.power = true;
      application.pause = false;
      snes.power();

      //warn if unsupported hardware detected
      string chip;
      if(cartridge.has_superfx())    chip = "SuperFX";
      else if(cartridge.has_sa1())   chip = "SA-1";
      else if(cartridge.has_st011()) chip = "ST011";
      else if(cartridge.has_st018()) chip = "ST018";
      else if(cartridge.has_dsp3())  chip = "DSP-3";
      if(chip != "") {
        QMessageBox::warning(winMain->window, "Warning", utf8()
        << "<p><b>Warning:</b><br>Unsupported " << chip << " chip detected. "
        << "It is unlikely that this title will work properly.</p>");
      }

      showMessage(utf8()
      << "Loaded " << cartridge.name()
      << (cartridge.patched() ? ", and applied UPS patch." : "."));
      winMain->window->setWindowTitle(utf8() << BSNES_TITLE << " - " << cartridge.name());
    } break;

    case UnloadCartridge: {
      if(cartridge.loaded() == false) break;  //no cart to unload?
      cartridge.unload();

      application.power = false;
      application.pause = true;

      showMessage(utf8() << "Unloaded " << cartridge.name() << ".");
      winMain->window->setWindowTitle(utf8() << BSNES_TITLE);
    } break;

    case PowerOn: {
      if(cartridge.loaded() == false || application.power == true) break;

      application.power = true;
      application.pause = false;
      snes.power();

      showMessage("Power on.");
    } break;

    case PowerOff: {
      if(cartridge.loaded() == false || application.power == false) break;

      application.power = false;
      application.pause = true;

      showMessage("Power off.");
    } break;

    case PowerCycle: {
      if(cartridge.loaded() == false) break;

      application.power = true;
      application.pause = false;
      snes.power();

      showMessage("System power was cycled.");
    } break;

    case Reset: {
      if(cartridge.loaded() == false || application.power == false) break;

      application.pause = false;
      snes.reset();

      showMessage("System was reset.");
    } break;
  }

  winMain->syncUi();
  winCodeEditor->dismiss();
  winCheatEditor->reloadList();
  winCheatEditor->syncUi();
}
