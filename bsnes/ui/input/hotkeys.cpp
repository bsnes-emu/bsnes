void InputMapper::poll_hotkeys(unsigned scancode, int16_t value) {
  //internal state
  static unsigned activeSlot = 1;
  static bool videoSync = false;
  static bool audioSync = false;

  if(value) {
    //key pressed
    if(mainWindow.focused() == false) return;

    //save states
    if(scancode == hotkeysGeneral.stateSave.scancode) {
      utility.saveState(activeSlot);
    }

    if(scancode == hotkeysGeneral.stateLoad.scancode) {
      utility.loadState(activeSlot);
    }

    if(scancode == hotkeysGeneral.stateDecrement.scancode) {
      activeSlot = (activeSlot == 1 ? 5 : activeSlot - 1);
      utility.showMessage({ "Slot ", activeSlot, " selected" });
    }

    if(scancode == hotkeysGeneral.stateIncrement.scancode) {
      activeSlot = (activeSlot == 5 ? 1 : activeSlot + 1);
      utility.showMessage({ "Slot ", activeSlot, " selected" });
    }

    //capture screenshot
    if(scancode == hotkeysGeneral.captureScreenshot.scancode) {
      interface.captureScreenshot = true;
    }

    //fullscreen
    if(scancode == hotkeysGeneral.fullscreenToggle.scancode) {
      utility.setFullScreen(!utility.fullScreen);
    }

    //mouse capture
    if(scancode == hotkeysGeneral.mouseCaptureToggle.scancode) {
      if(input.acquired() == false) {
        input.acquire();
      } else {
        input.unacquire();
      }
    }

    //pause
    if(scancode == hotkeysGeneral.pauseToggle.scancode) {
      application.pause = !application.pause;
    }

    //fast forward
    if(scancode == hotkeysGeneral.fastForward.scancode) {
      videoSync = config.video.synchronize;
      audioSync = config.audio.synchronize;
      video.set(Video::Synchronize, config.video.synchronize = false);
      audio.set(Audio::Synchronize, config.audio.synchronize = false);
      #if defined(PROFILE_COMPATIBILITY) || defined(PROFILE_PERFORMANCE)
      SNES::ppu.set_frameskip(9);
      #endif
    }

    //power cycle
    if(scancode == hotkeysGeneral.power.scancode) {
      mainWindow.systemPower.onTick();
    }

    //reset
    if(scancode == hotkeysGeneral.reset.scancode) {
      mainWindow.systemReset.onTick();
    }

    //exit emulator
    if(scancode == hotkeysGeneral.exitEmulator.scancode) {
      application.quit = true;
    }
  } else {
    //key released
    if(mainWindow.focused() == false) return;

    //fast forward
    if(scancode == hotkeysGeneral.fastForward.scancode) {
      video.set(Video::Synchronize, config.video.synchronize = videoSync);
      audio.set(Audio::Synchronize, config.audio.synchronize = audioSync);
      #if defined(PROFILE_COMPATIBILITY) || defined(PROFILE_PERFORMANCE)
      SNES::ppu.set_frameskip(0);
      #endif
    }
  }
}

void InputMapper::HotkeysGeneral::create(const char *deviceName, const char *configName) {
  name = deviceName;

  stateSave.name = "Save State";
  stateLoad.name = "Load State";
  stateDecrement.name = "Decrement State";
  stateIncrement.name = "Increment State";
  captureScreenshot.name = "Capture Screenshot";
  fullscreenToggle.name = "Fullscreen";
  mouseCaptureToggle.name = "Mouse Capture";
  pauseToggle.name = "Pause Emulation";
  fastForward.name = "Fast-Forward";
  power.name = "Power Cycle";
  reset.name = "Reset";
  exitEmulator.name = "Exit Emulator";

  append(&stateSave);
  append(&stateLoad);
  append(&stateDecrement);
  append(&stateIncrement);
  append(&captureScreenshot);
  append(&fullscreenToggle);
  append(&mouseCaptureToggle);
  append(&pauseToggle);
  append(&fastForward);
  append(&power);
  append(&reset);
  append(&exitEmulator);

  config.attach(stateSave.mapping = "KB0::F5", string("input.", configName, ".stateSave"));
  config.attach(stateLoad.mapping = "KB0::F7", string("input.", configName, ".stateLoad"));
  config.attach(stateDecrement.mapping = "KB0::F6", string("input.", configName, ".stateDecrement"));
  config.attach(stateIncrement.mapping = "KB0::F8", string("input.", configName, ".stateIncrement"));
  config.attach(captureScreenshot.mapping = "", string("input.", configName, ".captureScreenshot"));
  config.attach(fullscreenToggle.mapping = "KB0::F11", string("input.", configName, ".fullscreenToggle"));
  config.attach(mouseCaptureToggle.mapping = "KB0::F12", string("input.", configName, ".mouseCaptureToggle"));
  config.attach(pauseToggle.mapping = "KB0::P", string("input.", configName, ".pauseToggle"));
  config.attach(fastForward.mapping = "KB0::Tilde", string("input.", configName, ".fastForward"));
  config.attach(power.mapping = "", string("input.", configName, ".power"));
  config.attach(reset.mapping = "", string("input.", configName, ".reset"));
  config.attach(exitEmulator.mapping = "", string("input.", configName, ".exitEmulator"));
}

void InputMapper::create_hotkeys() {
  hotkeys.name = "Hotkeys";
  hotkeysGeneral.create("General", "hotkeys.general");
  hotkeys.append(&hotkeysGeneral);
}
