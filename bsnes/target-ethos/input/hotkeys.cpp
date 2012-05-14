void InputManager::appendHotkeys() {
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Fullscreen Mode";
    hotkey->mapping = "KB0::F11";

    hotkey->press = [] {
      utility->toggleFullScreen();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Mouse Capture";
    hotkey->mapping = "KB0::F12";

    hotkey->press = [] {
      input.acquired() ? input.unacquire() : input.acquire();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name = "Pause Emulation";
    hotkey->mapping = "KB0::P";

    hotkey->press = [] {
      application->pause = !application->pause;
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Fast Forward";
    hotkey->mapping = "KB0::Tilde";

    hotkey->press = [] {
      video.set(Video::Synchronize, false);
      audio.set(Audio::Synchronize, false);
    };

    hotkey->release = [] {
      video.set(Video::Synchronize, ::config->video.synchronize);
      audio.set(Audio::Synchronize, ::config->audio.synchronize);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Power Cycle";
    hotkey->mapping = "None";

    hotkey->press = [] {
      utility->power();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Soft Reset";
    hotkey->mapping = "None";

    hotkey->press = [] {
      utility->reset();
    };
  }

  static unsigned activeSlot = 1;

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Save State";
    hotkey->mapping = "KB0::F5";

    hotkey->press = [&] {
      utility->saveState(activeSlot);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Load State";
    hotkey->mapping = "KB0::F7";

    hotkey->press = [&] {
      utility->loadState(activeSlot);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Decrement Slot";
    hotkey->mapping = "KB0::F6";

    hotkey->press = [&] {
      if(--activeSlot == 0) activeSlot = 5;
      utility->showMessage({"Selected slot ", activeSlot});
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Increment Slot";
    hotkey->mapping = "KB0::F8";

    hotkey->press = [&] {
      if(++activeSlot == 6) activeSlot = 1;
      utility->showMessage({"Selected slot ", activeSlot});
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Close Emulator";
    hotkey->mapping = "None";

    hotkey->press = [] {
      application->quit = true;
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Toggle Tracer";
    hotkey->mapping = "None";

    hotkey->press = [&] {
      utility->tracerToggle();
    };
  }

  for(auto &hotkey : hotkeyMap) {
    string name = {"Hotkey::", hotkey->name};
    name.replace(" ", "");
    config.append(hotkey->mapping, name);
  }
}

void InputManager::pollHotkeys() {
  for(auto &hotkey : hotkeyMap) {
    bool state = hotkey->poll();
    if(hotkey->state == 0 && state == 1) if(hotkey->press) hotkey->press();
    if(hotkey->state == 1 && state == 0) if(hotkey->release) hotkey->release();
    hotkey->state = state;
  }
}
