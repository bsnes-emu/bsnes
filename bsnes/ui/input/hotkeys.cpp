void InputMapper::poll_hotkeys(unsigned scancode, int16_t value) {
  //internal state
  static unsigned activeSlot = 1;
  static bool videoSync = false;
  static bool audioSync = false;

  if(value) {
    //key pressed
    if(mainWindow.focused() == false) return;

    //save states
    if(scancode == keyboard(0)[Keyboard::F5]) {
      utility.saveState(activeSlot);
    }

    if(scancode == keyboard(0)[Keyboard::F7]) {
      utility.loadState(activeSlot);
    }

    if(scancode == keyboard(0)[Keyboard::F6]) {
      activeSlot = (activeSlot == 1 ? 5 : activeSlot - 1);
      utility.showMessage({ "Slot ", activeSlot, " selected" });
    }

    if(scancode == keyboard(0)[Keyboard::F8]) {
      activeSlot = (activeSlot == 5 ? 1 : activeSlot + 1);
      utility.showMessage({ "Slot ", activeSlot, " selected" });
    }

    //fullscreen
    if(scancode == keyboard(0)[Keyboard::F11]) {
      utility.setFullscreen(!utility.fullscreen);
    }

    //mouse capture
    if(scancode == keyboard(0)[Keyboard::F12]) {
      if(input.acquired() == false) {
        input.acquire();
      } else {
        input.unacquire();
      }
    }

    //pause
    if(scancode == keyboard(0)[Keyboard::P]) {
      application.pause = !application.pause;
    }

    //fast forward
    if(scancode == keyboard(0)[Keyboard::Tilde]) {
      videoSync = config.video.synchronize;
      audioSync = config.audio.synchronize;
      video.set(Video::Synchronize, config.video.synchronize = false);
      audio.set(Audio::Synchronize, config.audio.synchronize = false);
      #if defined(PROFILE_COMPATIBILITY) || defined(PROFILE_PERFORMANCE)
      SNES::ppu.set_frameskip(9);
      #endif
    }
  } else {
    //key released
    if(mainWindow.focused() == false) return;

    //fast forward
    if(scancode == keyboard(0)[Keyboard::Tilde]) {
      video.set(Video::Synchronize, config.video.synchronize = videoSync);
      audio.set(Audio::Synchronize, config.audio.synchronize = audioSync);
      #if defined(PROFILE_COMPATIBILITY) || defined(PROFILE_PERFORMANCE)
      SNES::ppu.set_frameskip(0);
      #endif
    }
  }
}
