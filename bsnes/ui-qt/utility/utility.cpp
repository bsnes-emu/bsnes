Utility utility;

#include "system-state.cpp"
#include "window.cpp"

void Utility::inputEvent(uint16_t scancode) {
  //release mouse capture if escape key is pressed on any keyboard
  for(unsigned i = 0; i < Keyboard::Count; i++) {
    if(scancode == keyboard(i)[Keyboard::Escape] && mapper().state(scancode)) {
      if(mainWindow->isActive() && input.acquired()) {
        input.unacquire();
        return;
      }
    }
  }
}

//display message in main window statusbar area for three seconds
void Utility::showMessage(const char *message) {
  mainWindow->statusBar->showMessage(string() << message, 3000);
}

//updates system state text at bottom-right of main window statusbar
void Utility::updateSystemState() {
  string text;

  if(SNES::cartridge.loaded() == false) {
    text = "No cartridge loaded";
  } else if(application.power == false) {
    text = "Power off";
  } else if(application.pause == true || application.autopause == true) {
    text = "Paused";
  } else if(interface.framesUpdated == true) {
    interface.framesUpdated = false;
    text << interface.framesExecuted;
    text << " fps";
  } else {
    //nothing to update
    return;
  }

  mainWindow->systemState->setText(string() << "  " << text << "  ");
}

void Utility::acquireMouse() {
  if(SNES::cartridge.loaded()) {
    if(SNES::config.controller_port1 == SNES::Input::Device::Mouse
    || SNES::config.controller_port2 == SNES::Input::Device::Mouse
    || SNES::config.controller_port2 == SNES::Input::Device::SuperScope
    || SNES::config.controller_port2 == SNES::Input::Device::Justifier
    || SNES::config.controller_port2 == SNES::Input::Device::Justifiers
    ) input.acquire();
  }
}

void Utility::unacquireMouse() {
  input.unacquire();
}

void Utility::updateAvSync() {
  video.set(Video::Synchronize, config().video.synchronize);
  audio.set(Audio::Synchronize, config().audio.synchronize);
}

void Utility::updateColorFilter() {
  filter.contrast = config().video.contrastAdjust;
  filter.brightness = config().video.brightnessAdjust;
  filter.gamma = 100 + config().video.gammaAdjust;
  filter.gamma_ramp = config().video.enableGammaRamp;
  filter.colortable_update();
}

void Utility::updatePixelShader() {
  string filedata;

  if(filedata.readfile(config().path.shader)) {
    video.set(Video::Shader, (const char*)filedata);
  } else {
    video.set(Video::Shader, (const char*)0);
  }
}

void Utility::updateHardwareFilter() {
  video.set(Video::Filter, config().video.context->hwFilter);
}

void Utility::updateSoftwareFilter() {
  filter.renderer = config().video.context->swFilter;
}

void Utility::updateEmulationSpeed() {
  config().system.speed = max(0, min(4, (signed)config().system.speed));

  double scale[] = {
    config().system.speedSlowest / 100.0,
    config().system.speedSlow    / 100.0,
    config().system.speedNormal  / 100.0,
    config().system.speedFast    / 100.0,
    config().system.speedFastest / 100.0,
  };
  unsigned outfreq = config().audio.outputFrequency;
  unsigned infreq  = config().audio.inputFrequency * scale[config().system.speed] + 0.5;

  audio.set(Audio::Resample, true);  //always resample (required for volume adjust + frequency scaler)
  audio.set(Audio::ResampleRatio, (double)infreq / (double)outfreq);
}

void Utility::updateControllers() {
  SNES::input.port_set_device(0, SNES::config.controller_port1);
  SNES::input.port_set_device(1, SNES::config.controller_port2);

  switch(config().input.port1) { default:
    case ControllerPort1::None: mapper().port1 = 0; break;
    case ControllerPort1::Gamepad: mapper().port1 = &Controllers::gamepad1; break;
    case ControllerPort1::Asciipad: mapper().port1 = &Controllers::asciipad1; break;
    case ControllerPort1::Multitap: mapper().port1 = &Controllers::multitap1; break;
    case ControllerPort1::Mouse: mapper().port1 = &Controllers::mouse1; break;
  }

  switch(config().input.port2) { default:
    case ControllerPort2::None: mapper().port2 = 0; break;
    case ControllerPort2::Gamepad: mapper().port2 = &Controllers::gamepad2; break;
    case ControllerPort2::Asciipad: mapper().port2 = &Controllers::asciipad2; break;
    case ControllerPort2::Multitap: mapper().port2 = &Controllers::multitap2; break;
    case ControllerPort2::Mouse: mapper().port2 = &Controllers::mouse2; break;
    case ControllerPort2::SuperScope: mapper().port2 = &Controllers::superscope; break;
    case ControllerPort2::Justifier: mapper().port2 = &Controllers::justifier1; break;
    case ControllerPort2::Justifiers: mapper().port2 = &Controllers::justifiers; break;
  }

  mainWindow->syncUi();
}
