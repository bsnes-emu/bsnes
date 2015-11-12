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
  if(config->video.scale == "Medium") videoScaleMedium.setChecked();
  if(config->video.scale == "Large") videoScaleLarge.setChecked();
  videoScaleSmall.setText("Small").onActivate([&] {
    config->video.scale = "Small";
    resizeViewport();
  });
  videoScaleMedium.setText("Medium").onActivate([&] {
    config->video.scale = "Medium";
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
  loadShaders();
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

  statusBar.setFont(Font().setBold());
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
  inputPort3.setVisible(false).reset();

  for(auto n : range(emulator->port)) {
    if(n >= 3) break;
    auto& port = emulator->port[n];
    auto& menu = (n == 0 ? inputPort1 : n == 1 ? inputPort2 : inputPort3);
    menu.setText(port.name);

    Group devices;
    for(auto& device : port.device) {
      MenuRadioItem item{&menu};
      item.setText(device.name).onActivate([=] {
        emulatorSettings->set({emulator->information.name, "/", port.name}, device.name);
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
    if(devices.objectCount() > 1) {
      string device = emulatorSettings->get({emulator->information.name, "/", port.name});
      for(auto object : devices.objects()) {
        if(auto item = dynamic_cast<mMenuRadioItem*>(object.data())) {
          if(item->text() == device) {
            item->setChecked().doActivate();
          }
        }
      }
      menu.setVisible();
    }
  }

  systemMenuSeparatorPorts.setVisible(inputPort1.visible() || inputPort2.visible() || inputPort3.visible());
}

auto Presentation::resizeViewport() -> void {
  signed width   = emulator ? emulator->information.width  : 256;
  signed height  = emulator ? emulator->information.height : 240;
  double stretch = emulator ? emulator->information.aspectRatio : 1.0;
  if(stretch != 1.0) {
    //aspect correction is always enabled in fullscreen mode
    if(!fullScreen() && !config->video.aspectCorrection) stretch = 1.0;
  }

  signed scale = 2;
  if(config->video.scale == "Small" ) scale = 2;
  if(config->video.scale == "Medium") scale = 3;
  if(config->video.scale == "Large" ) scale = 4;

  signed windowWidth = 0, windowHeight = 0;
  if(!fullScreen()) {
    windowWidth  = 256 * scale * (config->video.aspectCorrection ? 8.0 / 7.0 : 1.0);
    windowHeight = 240 * scale;
  } else {
    windowWidth  = geometry().width();
    windowHeight = geometry().height();
  }

  signed multiplier = min(windowWidth / (signed)(width * stretch), windowHeight / height);
  width = width * multiplier * stretch;
  height = height * multiplier;

  if(!fullScreen()) setSize({windowWidth, windowHeight});
  viewport.setGeometry({(windowWidth - width) / 2, (windowHeight - height) / 2, width, height});

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

auto Presentation::loadShaders() -> void {
  if(config->video.driver != "OpenGL") {
    videoShaderMenu.setVisible(false);
    return;
  }

  auto pathname = locate({localpath(), "tomoko/"}, "Video Shaders/");
  for(auto shader : directory::folders(pathname, "*.shader")) {
    MenuRadioItem item{&videoShaderMenu};
    item.setText(string{shader}.rtrim(".shader/", 1L)).onActivate([=] {
      config->video.shader = {pathname, shader};
      program->updateVideoFilter();
    });
    videoShaders.append(item);
  }

  videoShaderMenu.setText("Video Shaders");
  videoShaderNone.setChecked().setText("None").onActivate([=] {
    config->video.shader = "None";
    program->updateVideoFilter();
  });

  for(auto object : videoShaders.objects()) {
    if(auto radioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
      if(config->video.shader == string{pathname, radioItem->text(), ".shader/"}) {
        radioItem->setChecked();
      }
    }
  }
}
