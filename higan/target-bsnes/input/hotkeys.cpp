auto InputManager::bindHotkeys() -> void {
  static int stateSlot = 1;

  hotkeys.append(InputHotkey("Toggle Fullscreen Mode").onPress([] {
    presentation->toggleFullscreenMode();
  }));

  hotkeys.append(InputHotkey("Toggle Mouse Capture").onPress([] {
    input->acquired() ? input->release() : input->acquire();
  }));

  hotkeys.append(InputHotkey("Toggle Cheat Codes").onPress([] {
    toolsWindow->cheatEditor.enableCheats.setChecked(
      !toolsWindow->cheatEditor.enableCheats.checked()
    );
    toolsWindow->cheatEditor.enableCheats.doToggle();
  }));

  hotkeys.append(InputHotkey("Save State").onPress([&] {
    program->saveState({"quick/slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Load State").onPress([&] {
    program->loadState({"quick/slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Increment State Slot").onPress([&] {
    if(--stateSlot < 1) stateSlot = 9;
    program->showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Decrement State Slot").onPress([&] {
    if(++stateSlot > 9) stateSlot = 1;
    program->showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Capture Screenshot").onPress([] {
    program->captureScreenshot();
  }));

  hotkeys.append(InputHotkey("Fast Forward").onPress([] {
    video->setBlocking(false);
    audio->setBlocking(false);
  }).onRelease([] {
    video->setBlocking(settings["Video/Blocking"].boolean());
    audio->setBlocking(settings["Audio/Blocking"].boolean());
  }));

  hotkeys.append(InputHotkey("Pause Emulation").onPress([] {
    presentation->pauseEmulation.setChecked(!presentation->pauseEmulation.checked());
  }));

  hotkeys.append(InputHotkey("Frame Advance").onPress([] {
    presentation->frameAdvance.doActivate();
  }));

  hotkeys.append(InputHotkey("Reset Emulation").onPress([] {
    program->reset();
  }));

  hotkeys.append(InputHotkey("Quit Emulator").onPress([] {
    program->quit();
  }));

  for(auto& hotkey : hotkeys) {
    hotkey.path = string{"Hotkey/", hotkey.name}.replace(" ", "");
    hotkey.assignment = settings(hotkey.path).text();
    hotkey.bind();
  }
}

auto InputManager::pollHotkeys() -> void {
  if(!program->focused()) return;

  for(auto& hotkey : hotkeys) {
    auto state = hotkey.poll();
    if(hotkey.state == 0 && state == 1 && hotkey.press) hotkey.press();
    if(hotkey.state == 1 && state == 0 && hotkey.release) hotkey.release();
    hotkey.state = state;
  }
}
