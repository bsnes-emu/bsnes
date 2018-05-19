#include "../bsnes.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemMenu.setText("System");
  load.setText("Load Game ...").onActivate([&] {
    BrowserDialog dialog;
    dialog.setTitle("Load Game");
    dialog.setPath({Path::user(), "bsnes/"});
    dialog.setFilters({string{"SNES Games|*.sfc:*.smc"}});
    if(auto location = dialog.openFile()) {
      program->load(location);
    }
  });
  reset.setText("Reset").setEnabled(false).onActivate([&] {
    if(emulator->loaded()) emulator->reset();
  });
  controllerPort1.setText("Controller Port 1");
  controllerPort2.setText("Controller Port 2");
  for(auto& port : emulator->ports) {
    Menu* menu = nullptr;
    if(port.name == "Controller Port 1") menu = &controllerPort1;
    if(port.name == "Controller Port 2") menu = &controllerPort2;
    if(!menu) continue;

    Group devices;
    for(auto& device : port.devices) {
      if(device.name == "None") continue;
      MenuRadioItem item{menu};
      item.setText(device.name).onActivate([=] {
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
  }
  quit.setText("Quit").onActivate([&] { program->quit(); });

  settingsMenu.setText("Settings");
  viewMenu.setText("View");
  smallView.setText("Small").onActivate([&] {});
  mediumView.setText("Medium").onActivate([&] {});
  largeView.setText("Large").onActivate([&] {});
  aspectCorrection.setText("Aspect Correction").onToggle([&] {});
  integralScaling.setText("Integral Scaling").onToggle([&] {});
  muteAudio.setText("Mute Audio").setChecked(settings["Audio/Mute"].boolean()).onToggle([&] {
    settings["Audio/Mute"].setValue(muteAudio.checked());
  });
  configuration.setText("Configuration ...").onActivate([&] { settingsWindow->setVisible().setFocused(); });

  toolsMenu.setText("Tools");
  saveState.setText("Save State").setEnabled(false);
  loadState.setText("Load State").setEnabled(false);
  pauseEmulation.setText("Pause Emulation").onToggle([&] {
    if(pauseEmulation.checked()) audio->clear();
  });

  helpMenu.setText("Help");
  about.setText("About ...").onActivate([&] {
    aboutWindow->setCentered(*this).setVisible().setFocused();
  });

  onClose([&] {
    program->quit();
  });

  setTitle({"bsnes v", Emulator::Version});
  setBackgroundColor({0, 0, 0});
  setSize({512, 480});
  setCentered();

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::clearViewport() -> void {
  uint32_t* output;
  uint length;
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video->lock(output, length, width, height)) {
    for(uint y : range(height)) {
      auto line = output + y * (length >> 2);
      for(uint x : range(width)) *line++ = 0xff000000;
    }
    video->unlock();
    video->output();
  }
}
