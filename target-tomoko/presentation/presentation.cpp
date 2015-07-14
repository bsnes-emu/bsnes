#include "../tomoko.hpp"
Presentation* presentation = nullptr;

Presentation::Presentation() {
  presentation = this;

  libraryMenu.setText("Library");
  for(auto& emulator : program->emulators) {
    for(auto& media : emulator->media) {
      if(!media.bootable) continue;
      auto item = new MenuItem{&libraryMenu};
      item->setText({media.name, " ..."}).onActivate([=] {
        directory::create({config->library.location, media.name});
        auto location = BrowserDialog()
        .setTitle({"Load ", media.name})
        .setPath({config->library.location, media.name})
        .setFilters(string{media.name, "|*.", media.type})
        .openFolder();
        if(directory::exists(location)) {
          program->loadMedia(location);
        }
      });
      loadBootableMedia.append(item);
    }
  }

  systemMenu.setText("System").setVisible(false);
  powerSystem.setText("Power").onActivate([&] { program->powerCycle(); });
  resetSystem.setText("Reset").onActivate([&] { program->softReset(); });
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedia(); drawSplashScreen(); });

  settingsMenu.setText("Settings");
  videoScaleMenu.setText("Video Scale");
  if(config->video.scale == "Small") videoScaleSmall.setChecked();
  if(config->video.scale == "Normal") videoScaleNormal.setChecked();
  if(config->video.scale == "Large") videoScaleLarge.setChecked();
  videoScaleSmall.setText("Small").onActivate([&] {
    config->video.scale = "Small";
    resizeViewport();
  });
  videoScaleNormal.setText("Normal").onActivate([&] {
    config->video.scale = "Normal";
    resizeViewport();
  });
  videoScaleLarge.setText("Large").onActivate([&] {
    config->video.scale = "Large";
    resizeViewport();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(config->video.aspectCorrection).onToggle([&] {
    config->video.aspectCorrection = aspectCorrection.checked();
    resizeViewport();
  });
  videoFilterMenu.setText("Video Filter");
  if(config->video.filter == "None") videoFilterNone.setChecked();
  if(config->video.filter == "Blur") videoFilterBlur.setChecked();
  videoFilterNone.setText("None").onActivate([&] { config->video.filter = "None"; program->updateVideoFilter(); });
  videoFilterBlur.setText("Blur").onActivate([&] { config->video.filter = "Blur"; program->updateVideoFilter(); });
  colorEmulation.setText("Color Emulation").setChecked(config->video.colorEmulation).onToggle([&] {
    config->video.colorEmulation = colorEmulation.checked();
    program->updateVideoPalette();
  });
  maskOverscan.setText("Mask Overscan").setChecked(config->video.overscan.mask).onToggle([&] {
    config->video.overscan.mask = maskOverscan.checked();
  });
  synchronizeVideo.setText("Synchronize Video").setChecked(config->video.synchronize).onToggle([&] {
    config->video.synchronize = synchronizeVideo.checked();
    video->set(Video::Synchronize, config->video.synchronize);
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(config->audio.synchronize).onToggle([&] {
    config->audio.synchronize = synchronizeAudio.checked();
    audio->set(Audio::Synchronize, config->audio.synchronize);
  });
  muteAudio.setText("Mute Audio").setChecked(config->audio.mute).onToggle([&] {
    config->audio.mute = muteAudio.checked();
    program->dsp.setVolume(config->audio.mute ? 0.0 : 1.0);
  });
  showStatusBar.setText("Show Status Bar").setChecked(config->userInterface.showStatusBar).onToggle([&] {
    config->userInterface.showStatusBar = showStatusBar.checked();
    statusBar.setVisible(config->userInterface.showStatusBar);
    if(visible()) resizeViewport();
  });
  showConfiguration.setText("Configuration ...").onActivate([&] { settingsManager->show(2); });

  toolsMenu.setText("Tools").setVisible(false);
  saveStateMenu.setText("Save State");
  saveSlot1.setText("Slot 1").onActivate([&] { program->saveState(1); });
  saveSlot2.setText("Slot 2").onActivate([&] { program->saveState(2); });
  saveSlot3.setText("Slot 3").onActivate([&] { program->saveState(3); });
  saveSlot4.setText("Slot 4").onActivate([&] { program->saveState(4); });
  saveSlot5.setText("Slot 5").onActivate([&] { program->saveState(5); });
  loadStateMenu.setText("Load State");
  loadSlot1.setText("Slot 1").onActivate([&] { program->loadState(1); });
  loadSlot2.setText("Slot 2").onActivate([&] { program->loadState(2); });
  loadSlot3.setText("Slot 3").onActivate([&] { program->loadState(3); });
  loadSlot4.setText("Slot 4").onActivate([&] { program->loadState(4); });
  loadSlot5.setText("Slot 5").onActivate([&] { program->loadState(5); });
  cheatEditor.setText("Cheat Editor").onActivate([&] { toolsManager->show(0); });
  stateManager.setText("State Manager").onActivate([&] { toolsManager->show(1); });

  statusBar.setFont(Font::sans(8, "Bold"));
  statusBar.setVisible(config->userInterface.showStatusBar);

  onClose([&] { program->quit(); });

  setTitle({"higan v", Emulator::Version});
  setResizable(false);
  setBackgroundColor({0, 0, 0});
  resizeViewport();
  setCentered();
}

auto Presentation::updateEmulator() -> void {
  if(!emulator) return;
  resetSystem.setVisible(emulator->information.resettable);
  inputPort1.setVisible(false).reset();
  inputPort2.setVisible(false).reset();

  for(auto n : range(emulator->port)) {
    if(n >= 2) break;
    auto& port = emulator->port[n];
    auto& menu = (n == 0 ? inputPort1 : inputPort2);
    menu.setText(port.name);

    Group devices;
    for(auto& device : port.device) {
      MenuRadioItem item{&menu};
      item.setText(device.name).onActivate([=] {
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
    if(devices.objects() > 1) menu.setVisible();
  }

  systemMenuSeparatorPorts.setVisible(inputPort1.visible() || inputPort2.visible());
}

auto Presentation::resizeViewport() -> void {
  signed scale = 1;
  if(config->video.scale == "Small" ) scale = 1;
  if(config->video.scale == "Normal") scale = 2;
  if(config->video.scale == "Large" ) scale = 4;

  signed width  = 256;
  signed height = 240;
  if(emulator) {
    width  = emulator->information.width;
    height = emulator->information.height;
  }

  bool arc = config->video.aspectCorrection;

  if(fullScreen() == false) {
    signed windowWidth  = 256 * scale;
    signed windowHeight = 240 * scale;
    if(arc) windowWidth = windowWidth * 8 / 7;

    double stretch = (arc && emulator && emulator->information.aspectRatio != 1.0) ? 8.0 / 7.0 : 1.0;
    signed multiplier = min(windowWidth / (signed)(width * stretch), windowHeight / height);
    width = width * multiplier * stretch;
    height = height * multiplier;

    setSize({windowWidth, windowHeight});
    viewport.setGeometry({(windowWidth - width) / 2, (windowHeight - height) / 2, width, height});
  } else {
    auto desktop = Desktop::size();

    //aspect ratio correction is always enabled in fullscreen mode
    //note that below algorithm yields 7:6 ratio on 2560x(1440,1600) monitors
    //this is extremely close to the optimum 8:7 ratio
    //it is used so that linear interpolation isn't required
    //todo: we should handle other resolutions nicely as well
    unsigned multiplier = desktop.height() / height;
    width *= 1 + multiplier;
    height *= multiplier;

    signed x = (desktop.width() - width) / 2;
    signed y = (desktop.height() - height) / 2;

    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(width > desktop.width()) width = desktop.width();
    if(height > desktop.height()) height = desktop.height();

    viewport.setGeometry({x, y, width, height});
  }

  if(!emulator) drawSplashScreen();
}

auto Presentation::toggleFullScreen() -> void {
  if(fullScreen() == false) {
    menuBar.setVisible(false);
    statusBar.setVisible(false);
    setResizable(true);
    setFullScreen(true);
    if(!input->acquired()) input->acquire();
  } else {
    if(input->acquired()) input->release();
    setFullScreen(false);
    setResizable(false);
    menuBar.setVisible(true);
    statusBar.setVisible(config->userInterface.showStatusBar);
  }

  Application::processEvents();
  resizeViewport();
}

auto Presentation::drawSplashScreen() -> void {
  if(!video) return;
  uint32* output;
  unsigned length;
  if(video->lock(output, length, 256, 240)) {
    for(auto y : range(240)) {
      uint32* dp = output + y * (length >> 2);
      for(auto x : range(256)) *dp++ = 0xff000000;
    }
    video->unlock();
    video->refresh();
  }
}
