#include "cartridge.cpp"
#include "window.cpp"

void Utility::inputEvent(uint16_t code) {
  //if input capture assignment window is currently active, forward key-press event
  if(winInputCapture->activeObject) winInputCapture->inputEvent(code);

  //only match buttons being pressed down ...
  if(InputCode::isButton(code) == false) return;
  if(inputManager.state(code) == false) return;

  if(code == keyboard::escape && input.acquired()) {
    input.unacquire();
    return;  //do not trigger other UI actions that may be bound to escape key
  }

  if(winMain->window->isActiveWindow()) {
    bool resizeWindow = false;

    if(code == inputUiGeneral.loadCartridge.code) {
      string filename = selectCartridge();
      if(filename.length() > 0) loadCartridge(filename);
    }

    if(code == inputUiGeneral.pauseEmulation.code) {
      application.pause = !application.pause;
    }

    if(code == inputUiGeneral.resetSystem.code) {
      modifySystemState(Reset);
    }

    if(code == inputUiGeneral.powerCycleSystem.code) {
      modifySystemState(PowerCycle);
    }

    if(code == inputUiGeneral.lowerSpeed.code) {
      config.system.speed--;
      updateEmulationSpeed();
      winMain->syncUi();
    }

    if(code == inputUiGeneral.raiseSpeed.code) {
      config.system.speed++;
      updateEmulationSpeed();
      winMain->syncUi();
    }

    if(code == inputUiGeneral.toggleCheatSystem.code) {
      if(cheat.enabled() == false) {
        cheat.enable();
        showMessage("Cheat system enabled.");
      } else {
        cheat.disable();
        showMessage("Cheat system disabled.");
      }
    }

    if(code == inputUiGeneral.toggleFullscreen.code) {
      config.video.isFullscreen = !config.video.isFullscreen;
      updateFullscreenState();
      winMain->syncUi();
    }

    if(code == inputUiGeneral.toggleMenu.code) {
      winMain->window->menuBar()->setVisible(!winMain->window->menuBar()->isVisibleTo(winMain->window));
      resizeWindow = true;
    }

    if(code == inputUiGeneral.toggleStatus.code) {
      winMain->window->statusBar()->setVisible(!winMain->window->statusBar()->isVisibleTo(winMain->window));
      resizeWindow = true;
    }

    //prevent calling twice when toggleMenu == toggleStatus
    if(resizeWindow == true) {
      resizeMainWindow();
    }

    if(code == inputUiGeneral.exitEmulator.code) {
      application.terminate = true;
    }
  }
}

//display message in main window statusbar area for three seconds
void Utility::showMessage(const char *message) {
  winMain->window->statusBar()->showMessage(utf8() << message, 3000);
}

//updates system state text at bottom-right of main window statusbar
void Utility::updateSystemState() {
  string text;

  if(cartridge.loaded() == false) {
    text = "No cartridge loaded";
  } else if(application.power == false) {
    text = "Power off";
  } else if(application.pause == true || application.autopause == true) {
    text = "Paused";
  } else if(ppu.status.frames_updated == true) {
    ppu.status.frames_updated = false;
    text << (int)ppu.status.frames_executed;
    text << " fps";
  } else {
    //nothing to update
    return;
  }

  winMain->systemState->setText(utf8() << text);
}

void Utility::acquireMouse() {
  if(cartridge.loaded()) {
    if(snes.config.controller_port1 == SNES::Input::DeviceMouse
    || snes.config.controller_port2 == SNES::Input::DeviceMouse
    || snes.config.controller_port2 == SNES::Input::DeviceSuperScope
    || snes.config.controller_port2 == SNES::Input::DeviceJustifier
    || snes.config.controller_port2 == SNES::Input::DeviceJustifiers
    ) input.acquire();
  }
}

void Utility::unacquireMouse() {
  input.unacquire();
}

void Utility::updateAvSync() {
  video.set(Video::Synchronize, config.video.synchronize);
  audio.set(Audio::Synchronize, config.audio.synchronize);
}

void Utility::updateVideoMode() {
  if(config.video.context->region == 0) {
    snes.video.set_mode(SNES::Video::ModeNTSC);
  } else {
    snes.video.set_mode(SNES::Video::ModePAL);
  }
}

void Utility::updateColorFilter() {
  libfilter::colortable.set_format(libfilter::Colortable::RGB888);
  libfilter::colortable.set_contrast(config.video.contrastAdjust);
  libfilter::colortable.set_brightness(config.video.brightnessAdjust);
  libfilter::colortable.set_gamma(100 + config.video.gammaAdjust);
  libfilter::colortable.enable_gamma_ramp(config.video.enableGammaRamp);
  libfilter::colortable.update();
}

void Utility::updateHardwareFilter() {
  video.set(Video::Filter, config.video.context->hwFilter);
}

void Utility::updateSoftwareFilter() {
  libfilter::FilterInterface::FilterType type;
  switch(config.video.context->swFilter) { default:
    case 0: type = libfilter::FilterInterface::Direct; break;
    case 1: type = libfilter::FilterInterface::Scanline; break;
    case 2: type = libfilter::FilterInterface::Scale2x; break;
    case 3: type = libfilter::FilterInterface::HQ2x; break;
    case 4: type = libfilter::FilterInterface::NTSC; break;
  }
  libfilter::filter.set(type);
}

void Utility::updateEmulationSpeed() {
  config.system.speed = max(0, min(4, (signed)config.system.speed));

  double scale[] = { 0.50, 0.75, 1.00, 1.50, 2.00 };
  unsigned outfreq = config.audio.outputFrequency;
  unsigned infreq  = config.audio.inputFrequency * scale[config.system.speed] + 0.5;

  audio.set(Audio::Resample, outfreq != infreq);  //only resample when necessary
  audio.set(Audio::ResampleOutputFrequency, outfreq);
  audio.set(Audio::ResampleInputFrequency,  infreq);
}

void Utility::updateControllers() {
  snes.input.port_set_device(0, snes.config.controller_port1);
  snes.input.port_set_device(1, snes.config.controller_port2);
}
