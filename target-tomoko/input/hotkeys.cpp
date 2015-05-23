auto InputManager::appendHotkeys() -> void {
  { auto hotkey = new InputHotkey;
    hotkey->name = "Toggle Fullscreen";
    hotkey->action = [] {
      presentation->toggleFullScreen();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Toggle Mouse Capture";
    hotkey->action = [] {
      input.acquired() ? input.unacquire() : input.acquire();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Save State";
    hotkey->action = [] {
      program->saveState(0);
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Load State";
    hotkey->action = [] {
      program->loadState(0);
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Pause Emulation";
    hotkey->action = [] {
      program->pause = !program->pause;
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Power Cycle";
    hotkey->action = [] {
      program->powerCycle();
    };
    hotkeys.append(hotkey);
  }

  { auto hotkey = new InputHotkey;
    hotkey->name = "Soft Reset";
    hotkey->action = [] {
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
    if(hotkey->state == 0 && state == 1 && hotkey->action) hotkey->action();
    hotkey->state = state;
  }
}
