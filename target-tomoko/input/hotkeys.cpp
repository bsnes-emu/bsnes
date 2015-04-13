auto InputManager::appendHotkeys() -> void {
  {
    auto hotkey = new InputHotkey;
    hotkey->name = "Toggle Fullscreen";
    hotkey->action = [] {
      presentation->toggleFullScreen();
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
