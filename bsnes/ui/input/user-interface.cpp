void HotkeyGeneral::inputEvent(int16_t scancode, int16_t value) {
  if(scancode == saveState.scancode && value) {
    interface->saveState(activeSlot);
  }

  if(scancode == loadState.scancode && value) {
    interface->loadState(activeSlot);
  }

  if(scancode == decrementSlot.scancode && value) {
    if(--activeSlot == 0) activeSlot = 5;
    utility->showMessage({ "Selected slot ", activeSlot });
  }

  if(scancode == incrementSlot.scancode && value) {
    if(++activeSlot == 6) activeSlot = 1;
    utility->showMessage({ "Selected slot ", activeSlot });
  }

  if(scancode == toggleMouseCapture.scancode && value) {
    if(mainWindow->fullScreen() == false) {
      input.acquired() ? input.unacquire() : input.acquire();
    }
  }

  if(scancode == toggleFullScreen.scancode && value) {
    utility->toggleFullScreen();
  }

  if(scancode == pause.scancode && value) {
    application->pause = !application->pause;
  }

  if(scancode == turboMode.scancode) {
    static bool Vsync, Async;
    if(value) {
      Vsync = any_cast<bool>(video.get(Video::Synchronize));
      Async = any_cast<bool>(audio.get(Audio::Synchronize));
      video.set(Video::Synchronize, false);
      audio.set(Audio::Synchronize, false);
    } else {
      video.set(Video::Synchronize, Vsync);
      audio.set(Audio::Synchronize, Async);
    }
  }

  if(scancode == power.scancode && value) {
    interface->power();
  }

  if(scancode == reset.scancode && value) {
    interface->reset();
  }

  if(scancode == quit.scancode && value) {
    application->quit = true;
  }
}

HotkeyGeneral::HotkeyGeneral() {
  name = "General";

  saveState.name = "Save state";
  loadState.name = "Load state";
  decrementSlot.name = "Decrement state slot";
  incrementSlot.name = "Increment state slot";
  toggleMouseCapture.name = "Toggle mouse capture";
  toggleFullScreen.name = "Toggle fullscreen";
  pause.name = "Pause emulation";
  turboMode.name = "Turbo mode";
  power.name = "Power cycle";
  reset.name = "Reset";
  quit.name = "Close emulator";

  saveState.mapping = "KB0::F5";
  loadState.mapping = "KB0::F7";
  decrementSlot.mapping = "KB0::F6";
  incrementSlot.mapping = "KB0::F8";
  toggleMouseCapture.mapping = "KB0::F12";
  toggleFullScreen.mapping = "KB0::F11";
  pause.mapping = "KB0::P";
  turboMode.mapping = "KB0::Tilde";

  append(saveState);
  append(loadState);
  append(decrementSlot);
  append(incrementSlot);
  append(toggleMouseCapture);
  append(toggleFullScreen);
  append(pause);
  append(turboMode);
  append(power);
  append(reset);
  append(quit);

  activeSlot = 1;
}

//

void HotkeyInput::inputEvent(int16_t scancode, int16_t value) {
  general.inputEvent(scancode, value);
}

HotkeyInput::HotkeyInput() {
  name = "Hotkeys";
  append(general);
}

//

void UserInterfaceInput::inputEvent(int16_t scancode, int16_t value) {
  hotkey.inputEvent(scancode, value);
}

UserInterfaceInput::UserInterfaceInput() {
  name = "User Interface";
  append(hotkey);
}
