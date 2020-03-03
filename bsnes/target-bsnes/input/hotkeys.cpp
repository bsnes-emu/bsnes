auto InputHotkey::logic() const -> Logic {
  return inputManager.hotkeyLogic;
}

//

auto InputManager::bindHotkeys() -> void {
  static int stateSlot = 1;
  static double frequency = 48000.0;
  static double volume = 0.0;

  hotkeys.append(InputHotkey("Toggle Fullscreen").onPress([] {
    program.toggleVideoFullScreen();
  }));

  hotkeys.append(InputHotkey("Toggle Pseudo-Fullscreen").onPress([] {
    program.toggleVideoPseudoFullScreen();
  }));

  hotkeys.append(InputHotkey("Toggle Mouse Capture").onPress([] {
    input.acquired() ? input.release() : input.acquire();
  }));

  hotkeys.append(InputHotkey("Toggle Cheat Codes").onPress([] {
    cheatEditor.enableCheats.setChecked(!cheatEditor.enableCheats.checked()).doToggle();
  }));

  hotkeys.append(InputHotkey("Toggle Mute").onPress([] {
    presentation.muteAudio.setChecked(!presentation.muteAudio.checked()).doToggle();
  }));

  hotkeys.append(InputHotkey("Rewind").onPress([&] {
    if(!emulator->loaded() || program.fastForwarding) return;
    if(program.rewind.frequency == 0) {
      return program.showMessage("Please enable rewind support in Settings->Emulator first");
    }
    program.rewinding = true;
    program.rewindMode(Program::Rewind::Mode::Rewinding);
    volume = Emulator::audio.volume();
    if(settings.rewind.mute) {
      program.mute |= Program::Mute::Rewind;
    } else {
      Emulator::audio.setVolume(volume * 0.65);
    }
  }).onRelease([&] {
    program.rewinding = false;
    if(!emulator->loaded()) return;
    program.rewindMode(Program::Rewind::Mode::Playing);
    program.mute &= ~Program::Mute::Rewind;
    Emulator::audio.setVolume(volume);
  }));

  hotkeys.append(InputHotkey("Save State").onPress([&] {
    program.saveState({"Quick/Slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Load State").onPress([&] {
    program.loadState({"Quick/Slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Load Undo State").onPress([&] {
    program.loadState("Quick/Undo");
  }));

  hotkeys.append(InputHotkey("Load Redo State").onPress([&] {
    program.loadState("Quick/Redo");
  }));

  hotkeys.append(InputHotkey("Decrement State Slot").onPress([&] {
    if(--stateSlot < 1) stateSlot = 9;
    program.showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Increment State Slot").onPress([&] {
    if(++stateSlot > 9) stateSlot = 1;
    program.showMessage({"Selected state slot ", stateSlot});
  }));

  hotkeys.append(InputHotkey("Capture Screenshot").onPress([] {
    program.captureScreenshot();
  }));

  hotkeys.append(InputHotkey("Fast Forward").onPress([] {
    if(!emulator->loaded() || program.rewinding) return;
    program.fastForwarding = true;
    emulator->setFrameSkip(emulator->configuration("Hacks/PPU/Fast") == "true" ? settings.fastForward.frameSkip : 0);
    video.setBlocking(false);
    audio.setBlocking(settings.fastForward.limiter != 0);
    audio.setDynamic(false);
    frequency = Emulator::audio.frequency();
    volume = Emulator::audio.volume();
    if(settings.fastForward.limiter) {
      Emulator::audio.setFrequency(frequency / settings.fastForward.limiter);
    }
    if(settings.fastForward.mute) {
      program.mute |= Program::Mute::FastForward;
    } else if(settings.fastForward.limiter) {
      Emulator::audio.setVolume(volume * 0.65);
    }
  }).onRelease([] {
    program.fastForwarding = false;
    if(!emulator->loaded()) return;
    emulator->setFrameSkip(0);
    video.setBlocking(settings.video.blocking);
    audio.setBlocking(settings.audio.blocking);
    audio.setDynamic(settings.audio.dynamic);
    if(settings.fastForward.limiter) {
      Emulator::audio.setFrequency(frequency);
    }
    program.mute &= ~Program::Mute::FastForward;
    Emulator::audio.setVolume(volume);
  }));

  hotkeys.append(InputHotkey("Pause Emulation").onPress([] {
    if(presentation.runEmulation.checked()) {
      presentation.pauseEmulation.setChecked().doActivate();
    } else {
      //unpausing can also cancel frame advance mode
      presentation.runEmulation.setChecked().doActivate();
    }
  }));

  hotkeys.append(InputHotkey("Frame Advance").onPress([] {
    if(!presentation.frameAdvance.checked()) {
      //start frame advance if not currently frame advancing
      presentation.frameAdvance.setChecked().doActivate();
    }
    //advance one frame, even if we were currently paused when starting frame advance mode
    program.frameAdvanceLock = false;
  }));

  hotkeys.append(InputHotkey("Decrease HD Mode 7").onPress([] {
    int index = enhancementSettings.mode7Scale.selected().offset() - 1;
    if(index < 0) return;
    enhancementSettings.mode7Scale.item(index).setSelected();
    enhancementSettings.mode7Scale.doChange();
  }));

  hotkeys.append(InputHotkey("Increase HD Mode 7").onPress([] {
    int index = enhancementSettings.mode7Scale.selected().offset() + 1;
    if(index >= enhancementSettings.mode7Scale.itemCount()) return;
    enhancementSettings.mode7Scale.item(index).setSelected();
    enhancementSettings.mode7Scale.doChange();
  }));

  hotkeys.append(InputHotkey("Toggle Supersampling").onPress([] {
    enhancementSettings.mode7Supersample.setChecked(!enhancementSettings.mode7Supersample.checked()).doToggle();
  }));

  hotkeys.append(InputHotkey("Reset Emulation").onPress([] {
    program.reset();
  }));

  hotkeys.append(InputHotkey("Quit Emulator").onPress([] {
    program.quit();
  }));

  for(auto& hotkey : hotkeys) {
    hotkey.path = string{"Hotkey/", hotkey.name}.replace(" ", "");
    auto assignments = settings(hotkey.path).text().split(";");
    for(uint index : range(BindingLimit)) hotkey.assignments[index] = assignments(index);
    hotkey.bind();
  }
}

auto InputManager::pollHotkeys() -> void {
  if(Application::modal() || !program.focused()) return;

  for(auto& hotkey : hotkeys) {
    auto state = hotkey.poll();
    if(hotkey.state == 0 && state == 1 && hotkey.press) hotkey.press();
    if(hotkey.state == 1 && state == 0 && hotkey.release) hotkey.release();
    hotkey.state = state;
  }
}
