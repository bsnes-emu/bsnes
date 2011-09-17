void HotkeyGeneral::inputEvent(int16_t scancode, int16_t value) {
  if(scancode == toggleFullScreen.scancode && value) {
    utility->toggleFullScreen();
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
}

HotkeyGeneral::HotkeyGeneral() {
  name = "General";

  toggleFullScreen.name = "Toggle Fullscreen";
  turboMode.name = "Turbo Mode";

  toggleFullScreen.mapping = "KB0::F11";
  turboMode.mapping = "KB0::Tilde";

  append(toggleFullScreen);
  append(turboMode);
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
