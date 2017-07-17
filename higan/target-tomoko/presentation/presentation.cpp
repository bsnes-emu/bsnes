#include "../tomoko.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  libraryMenu.setText("Library");
  string_vector manufacturers;
  for(auto& emulator : program->emulators) {
    if(!manufacturers.find(emulator->information.manufacturer)) {
      manufacturers.append(emulator->information.manufacturer);
    }
  }
  for(auto& manufacturer : manufacturers) {
    Menu manufacturerMenu{&libraryMenu};
    manufacturerMenu.setText(manufacturer);
    for(auto& emulator : program->emulators) {
      if(emulator->information.manufacturer != manufacturer) continue;
      for(auto& medium : emulator->media) {
        auto item = new MenuItem{&manufacturerMenu};
        item->setText({medium.name, " ..."}).onActivate([=] {
          program->loadMedium(*emulator, medium);
        });
      }
    }
  }
  //add icarus menu options -- but only if icarus binary is present
  if(execute("icarus", "--name").output.strip() == "icarus") {
    libraryMenu.append(MenuSeparator());
    libraryMenu.append(MenuItem().setText("Load ROM File ...").onActivate([&] {
      audio->clear();
      if(auto location = execute("icarus", "--import")) {
        program->mediumQueue.append(location.output.strip());
        program->loadMedium();
      }
    }));
    libraryMenu.append(MenuItem().setText("Import ROM Files ...").onActivate([&] {
      invoke("icarus");
    }));
  }

  systemMenu.setText("System").setVisible(false);
  reloadSystem.setText("Power Cycle").onActivate([&] { program->powerCycle(); });
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedium(); });

  settingsMenu.setText("Settings");
  videoScaleMenu.setText("Video Scale");
  videoScaleSmall.setText("Small").onActivate([&] {
    settings["Video/Windowed/Scale"].setValue("Small");
    resizeViewport();
  });
  videoScaleMedium.setText("Medium").onActivate([&] {
    settings["Video/Windowed/Scale"].setValue("Medium");
    resizeViewport();
  });
  videoScaleLarge.setText("Large").onActivate([&] {
    settings["Video/Windowed/Scale"].setValue("Large");
    resizeViewport();
  });
  videoEmulationMenu.setText("Video Emulation");
  blurEmulation.setText("Blurring").setChecked(settings["Video/BlurEmulation"].boolean()).onToggle([&] {
    settings["Video/BlurEmulation"].setValue(blurEmulation.checked());
    if(emulator) emulator->set("Blur Emulation", blurEmulation.checked());
  });
  colorEmulation.setText("Colors").setChecked(settings["Video/ColorEmulation"].boolean()).onToggle([&] {
    settings["Video/ColorEmulation"].setValue(colorEmulation.checked());
    if(emulator) emulator->set("Color Emulation", colorEmulation.checked());
  });
  scanlineEmulation.setText("Scanlines").setChecked(settings["Video/ScanlineEmulation"].boolean()).setVisible(false).onToggle([&] {
    settings["Video/ScanlineEmulation"].setValue(scanlineEmulation.checked());
    if(emulator) emulator->set("Scanline Emulation", scanlineEmulation.checked());
  });
  videoShaderMenu.setText("Video Shader");
  videoShaderNone.setText("None").onActivate([&] {
    settings["Video/Shader"].setValue("None");
    program->updateVideoShader();
  });
  videoShaderBlur.setText("Blur").onActivate([&] {
    settings["Video/Shader"].setValue("Blur");
    program->updateVideoShader();
  });
  loadShaders();
  synchronizeVideo.setText("Synchronize Video").setChecked(settings["Video/Synchronize"].boolean()).setVisible(false).onToggle([&] {
    settings["Video/Synchronize"].setValue(synchronizeVideo.checked());
    video->setBlocking(synchronizeVideo.checked());
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(settings["Audio/Synchronize"].boolean()).onToggle([&] {
    settings["Audio/Synchronize"].setValue(synchronizeAudio.checked());
    audio->setBlocking(synchronizeAudio.checked());
  });
  muteAudio.setText("Mute Audio").setChecked(settings["Audio/Mute"].boolean()).onToggle([&] {
    settings["Audio/Mute"].setValue(muteAudio.checked());
    program->updateAudioEffects();
  });
  showStatusBar.setText("Show Status Bar").setChecked(settings["UserInterface/ShowStatusBar"].boolean()).onToggle([&] {
    settings["UserInterface/ShowStatusBar"].setValue(showStatusBar.checked());
    statusBar.setVisible(showStatusBar.checked());
    if(visible()) resizeViewport();
  });
  showConfiguration.setText("Configuration ...").onActivate([&] {
    //if no emulation core active; default to hotkeys panel
    if(!emulator) return settingsManager->show(3);

    //default to input panel with current core's input settings active
    for(auto item : settingsManager->input.emulatorList.items()) {
      if(systemMenu.text() == item.text()) {
        item.setSelected();
        settingsManager->input.emulatorList.doChange();
        break;
      }
    }
    settingsManager->show(2);
  });

  toolsMenu.setText("Tools").setVisible(false);
  saveQuickStateMenu.setText("Save Quick State");
  saveSlot1.setText("Slot 1").onActivate([&] { program->saveState(1); });
  saveSlot2.setText("Slot 2").onActivate([&] { program->saveState(2); });
  saveSlot3.setText("Slot 3").onActivate([&] { program->saveState(3); });
  saveSlot4.setText("Slot 4").onActivate([&] { program->saveState(4); });
  saveSlot5.setText("Slot 5").onActivate([&] { program->saveState(5); });
  loadQuickStateMenu.setText("Load Quick State");
  loadSlot1.setText("Slot 1").onActivate([&] { program->loadState(1); });
  loadSlot2.setText("Slot 2").onActivate([&] { program->loadState(2); });
  loadSlot3.setText("Slot 3").onActivate([&] { program->loadState(3); });
  loadSlot4.setText("Slot 4").onActivate([&] { program->loadState(4); });
  loadSlot5.setText("Slot 5").onActivate([&] { program->loadState(5); });
  cheatEditor.setText("Cheat Editor").onActivate([&] { toolsManager->show(0); });
  stateManager.setText("State Manager").onActivate([&] { toolsManager->show(1); });
  manifestViewer.setText("Manifest Viewer").onActivate([&] { toolsManager->show(2); });

  helpMenu.setText("Help");
  documentation.setText("Documentation ...").onActivate([&] {
    invoke("http://doc.byuu.org/higan/");
  });
  about.setText("About ...").onActivate([&] {
    aboutWindow->setVisible().setFocused();
  });

  statusBar.setFont(Font().setBold());
  statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());

  viewport.setDroppable().onDrop([&](auto locations) {
    if(!directory::exists(locations(0))) return;
    program->mediumQueue.append(locations(0));
    program->loadMedium();
  });

  onSize([&] {
    resizeViewport(false);
  });

  onClose([&] {
    program->quit();
  });

  setTitle({"higan v", Emulator::Version});
  setBackgroundColor({0, 0, 0});
  resizeViewport();
  setCentered();

  #if defined(PLATFORM_WINDOWS)
  Application::Windows::onModalChange([](bool modal) { if(modal && audio) audio->clear(); });
  #endif

  #if defined(PLATFORM_MACOSX)
  showConfigurationSeparator.setVisible(false);
  showConfiguration.setVisible(false);
  about.setVisible(false);
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onPreferences([&] { showConfiguration.doActivate(); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::updateEmulator() -> void {
  if(!emulator) return;
  inputPort1.setVisible(false).reset();
  inputPort2.setVisible(false).reset();
  inputPort3.setVisible(false).reset();

  for(auto n : range(emulator->ports)) {
    if(n >= 3) break;
    auto& port = emulator->ports[n];
    auto& menu = (n == 0 ? inputPort1 : n == 1 ? inputPort2 : inputPort3);
    menu.setText(port.name);

    Group devices;
    for(auto& device : port.devices) {
      MenuRadioItem item{&menu};
      item.setText(device.name).onActivate([=] {
        auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
        settings[path].setValue(device.name);
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
    if(devices.objectCount() > 1) {
      auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
      auto device = settings(path).text();
      for(auto item : devices.objects<MenuRadioItem>()) {
        if(item.text() == device) item.setChecked();
      }
      menu.setVisible();
    }
  }

  systemMenuSeparatorPorts.setVisible(inputPort1.visible() || inputPort2.visible() || inputPort3.visible());

  emulator->set("Blur Emulation", blurEmulation.checked());
  emulator->set("Color Emulation", colorEmulation.checked());
  emulator->set("Scanline Emulation", scanlineEmulation.checked());
}

auto Presentation::clearViewport() -> void {
  if(!video) return;

  uint32_t* output;
  uint length = 0;
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video->lock(output, length, width, height)) {
    for(uint y : range(height)) {
      auto dp = output + y * (length >> 2);
      for(uint x : range(width)) *dp++ = 0xff000000;
    }

    video->unlock();
    video->output();
  }
}

auto Presentation::resizeViewport(bool resizeWindow) -> void {
  //clear video area before resizing to avoid seeing distorted video momentarily
  clearViewport();

  uint viewportWidth = geometry().width();
  uint viewportHeight = geometry().height();

  double emulatorWidth = 320;
  double emulatorHeight = 240;
  double aspectCorrection = 1.0;
  if(emulator) {
    auto resolution = emulator->videoResolution();
    emulatorWidth = resolution.width;
    emulatorHeight = resolution.height;
    aspectCorrection = resolution.aspectCorrection;
    if(emulator->information.overscan) {
      uint overscanHorizontal = settings["Video/Overscan/Horizontal"].natural();
      uint overscanVertical = settings["Video/Overscan/Vertical"].natural();
      emulatorWidth -= overscanHorizontal * 2;
      emulatorHeight -= overscanVertical * 2;
    }
  }

  if(!fullScreen()) {
    if(settings["Video/Windowed/AspectCorrection"].boolean()) emulatorWidth *= aspectCorrection;

    if(resizeWindow) {
      string viewportScale = "640x480";
      if(settings["Video/Windowed/Scale"].text() == "Small") viewportScale = settings["Video/Windowed/Scale/Small"].text();
      if(settings["Video/Windowed/Scale"].text() == "Medium") viewportScale = settings["Video/Windowed/Scale/Medium"].text();
      if(settings["Video/Windowed/Scale"].text() == "Large") viewportScale = settings["Video/Windowed/Scale/Large"].text();
      auto resolution = viewportScale.isplit("x", 1L);
      viewportWidth = resolution(0).natural();
      viewportHeight = resolution(1).natural();
    }

    if(settings["Video/Windowed/Adaptive"].boolean() && resizeWindow) {
      uint multiplier = min(viewportWidth / emulatorWidth, viewportHeight / emulatorHeight);
      emulatorWidth *= multiplier;
      emulatorHeight *= multiplier;
      setSize({viewportWidth = emulatorWidth, viewportHeight = emulatorHeight});
    } else if(settings["Video/Windowed/IntegralScaling"].boolean()) {
      uint multiplier = min(viewportWidth / emulatorWidth, viewportHeight / emulatorHeight);
      emulatorWidth *= multiplier;
      emulatorHeight *= multiplier;
      if(resizeWindow) setSize({viewportWidth, viewportHeight});
    } else {
      double multiplier = min(viewportWidth / emulatorWidth, viewportHeight / emulatorHeight);
      emulatorWidth *= multiplier;
      emulatorHeight *= multiplier;
      if(resizeWindow) setSize({viewportWidth, viewportHeight});
    }
  } else {
    if(settings["Video/Fullscreen/AspectCorrection"].boolean()) emulatorWidth *= aspectCorrection;

    if(settings["Video/Fullscreen/IntegralScaling"].boolean()) {
      uint multiplier = min(viewportWidth / emulatorWidth, viewportHeight / emulatorHeight);
      emulatorWidth *= multiplier;
      emulatorHeight *= multiplier;
    } else {
      double multiplier = min(viewportWidth / emulatorWidth, viewportHeight / emulatorHeight);
      emulatorWidth *= multiplier;
      emulatorHeight *= multiplier;
    }
  }

  viewport.setGeometry({
    (viewportWidth - emulatorWidth) / 2, (viewportHeight - emulatorHeight) / 2,
    emulatorWidth, emulatorHeight
  });

  //clear video area again to ensure entire viewport area has been painted in
  clearViewport();
}

auto Presentation::toggleFullScreen() -> void {
  if(!fullScreen()) {
    menuBar.setVisible(false);
    statusBar.setVisible(false);
    setFullScreen(true);
    video->setExclusive(settings["Video/Fullscreen/Exclusive"].boolean());
    if(!input->acquired()) input->acquire();
  } else {
    if(input->acquired()) input->release();
    video->setExclusive(false);
    setFullScreen(false);
    menuBar.setVisible(true);
    statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());
  }
  resizeViewport();
}

auto Presentation::loadShaders() -> void {
  auto pathname = locate("Video Shaders/");

  if(settings["Video/Driver"].text() == "OpenGL") {
    for(auto shader : directory::folders(pathname, "*.shader")) {
      if(videoShaders.objectCount() == 2) videoShaderMenu.append(MenuSeparator());
      MenuRadioItem item{&videoShaderMenu};
      item.setText(string{shader}.trimRight(".shader/", 1L)).onActivate([=] {
        settings["Video/Shader"].setValue({pathname, shader});
        program->updateVideoShader();
      });
      videoShaders.append(item);
    }
  }

  if(settings["Video/Shader"].text() == "None") videoShaderNone.setChecked();
  if(settings["Video/Shader"].text() == "Blur") videoShaderBlur.setChecked();

  for(auto radioItem : videoShaders.objects<MenuRadioItem>()) {
    if(settings["Video/Shader"].text() == string{pathname, radioItem.text(), ".shader/"}) {
      radioItem.setChecked();
    }
  }
}
