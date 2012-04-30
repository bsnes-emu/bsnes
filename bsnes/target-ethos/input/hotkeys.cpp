void InputManager::appendHotkeys() {
  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Fast Forward";
    hotkey->mapping = "KB0::Tilde";
    hotkey->logic   = 1;
    hotkeyMap.append(hotkey);

    hotkey->press = [] {
      video.set(Video::Synchronize, false);
      audio.set(Audio::Synchronize, false);
    };

    hotkey->release = [] {
      video.set(Video::Synchronize, false);
      audio.set(Audio::Synchronize, true);
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Power Cycle";
    hotkey->mapping = "None";
    hotkey->logic   = 1;
    hotkeyMap.append(hotkey);

    hotkey->press = [] {
      utility->power();
    };
  }

  {
    auto hotkey = new HotkeyInput;
    hotkey->name    = "Soft Reset";
    hotkey->mapping = "None";
    hotkey->logic   = 1;
    hotkeyMap.append(hotkey);

    hotkey->press = [] {
      utility->reset();
    };
  }

  for(auto &hotkey : hotkeyMap) {
    config.append(hotkey->mapping, hotkey->name);
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
