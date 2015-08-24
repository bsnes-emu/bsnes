auto InputManager::appendHotkeys() -> void {
  { auto hotkey = new InputHotkey;
    hotkey->name = "Toggle Fullscreen";
    hotkey->press = [] {
      presentation->toggleFullScreen();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Toggle Mouse Capture";
    hotkey->press = [] {
      input->acquired() ? input->release() : input->acquire();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Save State";
    hotkey->press = [] {
      program->saveState(0);
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Load State";
    hotkey->press = [] {
      program->loadState(0);
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Pause Emulation";
    hotkey->press = [] {
      program->pause = !program->pause;
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Fast Forward";
    hotkey->press = [] {
      video->set(Video::Synchronize, false);
      audio->set(Audio::Synchronize, false);
    };
    hotkey->release = [] {
      video->set(Video::Synchronize, ::config->video.synchronize);
      audio->set(Audio::Synchronize, ::config->audio.synchronize);
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Power Cycle";
    hotkey->press = [] {
      program->powerCycle();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Soft Reset";
    hotkey->press = [] {
      program->softReset();
    };
    hotkeys.append(hotkey);
  }

  Configuration::Node nodeHotkeys;
  for(auto& hotkey : hotkeys) {
    nodeHotkeys.append(hotkey->assignment, string{hotkey->name}.replace(" ", ""));
  }
  config.append(nodeHotkeys, "Hotkeys");
}

auto InputManager::pollHotkeys() -> void {
  for(auto& hotkey : hotkeys) {
    int16 state = hotkey->poll();
    if(hotkey->state == 0 && state == 1 && hotkey->press) hotkey->press();
    if(hotkey->state == 1 && state == 0 && hotkey->release) hotkey->release();
    hotkey->state = state;
  }
}
