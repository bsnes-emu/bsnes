#include "../higan.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemsMenu.setText("Systems");

  systemMenu.setVisible(false);

  settingsMenu.setText("Settings");
  sizeMenu.setText("Size");
  updateSizeMenu();
  outputMenu.setText("Output");
  centerViewport.setText("Center").onActivate([&] {
    settings["View/Output"].setValue("Center");
    resizeViewport();
  });
  scaleViewport.setText("Scale").onActivate([&] {
    settings["View/Output"].setValue("Scale");
    resizeViewport();
  });
  stretchViewport.setText("Stretch").onActivate([&] {
    settings["View/Output"].setValue("Stretch");
    resizeViewport();
  });
  if(settings["View/Output"].text() == "Center") centerViewport.setChecked();
  if(settings["View/Output"].text() == "Scale") scaleViewport.setChecked();
  if(settings["View/Output"].text() == "Stretch") stretchViewport.setChecked();
  adaptiveSizing.setText("Adaptive Sizing").setChecked(settings["View/Adaptive"].boolean()).onToggle([&] {
    settings["View/Adaptive"].setValue(adaptiveSizing.checked());
    resizeWindow();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(settings["View/AspectCorrection"].boolean()).onToggle([&] {
    settings["View/AspectCorrection"].setValue(aspectCorrection.checked());
    resizeWindow();
  });
  showOverscanArea.setText("Show Overscan Area").setChecked(settings["View/Overscan"].boolean()).onToggle([&] {
    settings["View/Overscan"].setValue(showOverscanArea.checked());
    resizeWindow();
  });
  videoEmulationMenu.setText("Emulation");
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
  videoShaderMenu.setText("Shader");
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
  showStatusBar.setText("Show Status Bar").setChecked(settings["View/StatusBar"].boolean()).onToggle([&] {
    settings["View/StatusBar"].setValue(showStatusBar.checked());
    if(!showStatusBar.checked()) {
      layout.remove(statusLayout);
    } else {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    if(visible()) resizeWindow();
  });
  showSystemSettings.setIcon(Icon::Device::Storage).setText("Systems ...").onActivate([&] { settingsManager->show(0); });
  showVideoSettings.setIcon(Icon::Device::Display).setText("Video ...").onActivate([&] { settingsManager->show(1); });
  showAudioSettings.setIcon(Icon::Device::Speaker).setText("Audio ...").onActivate([&] { settingsManager->show(2); });
  showInputSettings.setIcon(Icon::Device::Joypad).setText("Input ...").onActivate([&] {
    if(emulator) {
      //default input panel to current core's input settings
      for(auto item : settingsManager->input.emulatorList.items()) {
        if(systemMenu.text() == item.text()) {
          item.setSelected();
          settingsManager->input.emulatorList.doChange();
          break;
        }
      }
    }
    settingsManager->show(3);
  });
  showHotkeySettings.setIcon(Icon::Device::Keyboard).setText("Hotkeys ...").onActivate([&] { settingsManager->show(4); });
  showAdvancedSettings.setIcon(Icon::Action::Settings).setText("Advanced ...").onActivate([&] { settingsManager->show(5); });

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
  pauseEmulation.setText("Pause Emulation").onToggle([&] { program->togglePause(); });
  cheatEditor.setIcon(Icon::Edit::Replace).setText("Cheat Editor ...").onActivate([&] { toolsManager->show(0); });
  stateManager.setIcon(Icon::Application::FileManager).setText("State Manager ...").onActivate([&] { toolsManager->show(1); });
  manifestViewer.setIcon(Icon::Emblem::Text).setText("Manifest Viewer ...").onActivate([&] { toolsManager->show(2); });
  gameNotes.setIcon(Icon::Emblem::Text).setText("Game Notes ...").onActivate([&] { toolsManager->show(3); });

  helpMenu.setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan/");
  });
  credits.setIcon(Icon::Application::Browser).setText("Credits ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan/credits/");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    aboutWindow->setCentered(*this).setVisible().setFocused();
  });

  viewport.setDroppable().onDrop([&](vector<string> locations) {
    if(!locations || !directory::exists(locations.first())) return;
    program->gameQueue.append(locations.first());
    program->load();
  }).onSize([&] {
    configureViewport();
  });

  iconLayout.setAlignment(0.0);
  image icon{Resource::Icon};
  icon.alphaBlend(0x000000);
  iconCanvas.setIcon(icon);

  if(!settings["View/StatusBar"].boolean()) {
    layout.remove(statusLayout);
  }

  auto font = Font().setBold();
  auto back = Color{ 32,  32,  32};
  auto fore = Color{255, 255, 255};

  spacerLeft.setBackgroundColor(back);

  statusMessage.setFont(font);
  statusMessage.setAlignment(0.0);
  statusMessage.setBackgroundColor(back);
  statusMessage.setForegroundColor(fore);

  statusInfo.setFont(font);
  statusInfo.setAlignment(1.0);
  statusInfo.setBackgroundColor(back);
  statusInfo.setForegroundColor(fore);
  statusInfo.setText("Unloaded");

  spacerRight.setBackgroundColor(back);

  onSize([&] {
    resizeViewport();
  });

  onClose([&] {
    program->quit();
  });

  settings["View/Multiplier"].setValue(2);

  setTitle({"higan v", Emulator::Version});
  setBackgroundColor({0, 0, 0});
  resizeWindow();
  setCentered();

  #if defined(PLATFORM_MACOS)
  about.setVisible(false);
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onPreferences([&] { showInputSettings.doActivate(); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::updateEmulatorMenu() -> void {
  if(!emulator) return;
  auto information = emulator->information();

  systemMenu.reset();
  for(auto& port : emulator->ports()) {
    Menu menu{&systemMenu};
    menu.setProperty("portID", port.id);
    menu.setText(port.name);
    if(port.name.beginsWith("Expansion") || port.name.beginsWith("Extension")) {
      menu.setIcon(Icon::Device::Storage);
    } else {
      menu.setIcon(Icon::Device::Joypad);
    }

    auto path = string{information.name, "/", port.name}.replace(" ", "");
    auto deviceName = settings(path).text();
    auto deviceID = emulator->connected(port.id);

    Group devices;
    for(auto& device : emulator->devices(port.id)) {
      MenuRadioItem item{&menu};
      item.setProperty("deviceID", device.id);
      item.setText(device.name);
      item.onActivate([=] {
        settings(path).setValue(device.name);
        emulator->connect(port.id, device.id);
        updateEmulatorDeviceSelections();
      });
      devices.append(item);

      if(deviceName == device.name) item.doActivate();
      if(!deviceName && deviceID == device.id) item.doActivate();
    }

    if(devices.objectCount() == 0) {
      menu.setVisible(false);
    }
  }

  if(systemMenu.actionCount()) {
    systemMenu.append(MenuSeparator());
  }

  if(information.resettable) {
    systemMenu.append(MenuItem().setText("Soft Reset").setIcon(Icon::Action::Refresh).onActivate([&] {
      program->softReset();
    }));
  }

  systemMenu.append(MenuItem().setText("Power Cycle").setIcon(Icon::Action::Refresh).onActivate([&] {
    program->powerCycle();
  }));

  systemMenu.append(MenuItem().setText("Unload").setIcon(Icon::Media::Eject).onActivate([&] {
    program->unload();
  }));

  updateEmulatorDeviceSelections();
}

auto Presentation::updateEmulatorDeviceSelections() -> void {
  if(!emulator) return;

  for(auto& port : emulator->ports()) {
    for(auto& action : systemMenu->actions()) {
      auto portID = action.property("portID");
      if(portID && portID.natural() == port.id) {
        if(auto menu = action.cast<Menu>()) {
          auto deviceID = emulator->connected(port.id);
          for(auto& action : menu.actions()) {
            if(auto item = action.cast<MenuRadioItem>()) {
              if(item.property("deviceID").natural() == deviceID) {
                item.setChecked();
              }
            }
          }
        }
      }
    }
  }
}

auto Presentation::updateSizeMenu() -> void {
  assert(sizeMenu.actionCount() == 0);  //should only be called once

  //determine the largest multiplier that can be used by the largest monitor found
  uint height = 1;
  for(uint monitor : range(Monitor::count())) {
    height = max(height, Monitor::workspace(monitor).height());
  }

  uint multipliers = max(1, height / 240);
  for(uint multiplier : range(1, multipliers + 1)) {
    MenuRadioItem item{&sizeMenu};
    item.setProperty("multiplier", multiplier);
    item.setText({multiplier, "x (", 240 * multiplier, "p)"});
    item.onActivate([=] {
      settings["View/Multiplier"].setValue(multiplier);
      resizeWindow();
    });
    sizeGroup.append(item);
  }

  for(auto item : sizeGroup.objects<MenuRadioItem>()) {
    if(settings["View/Multiplier"].natural() == item.property("multiplier").natural()) {
      item.setChecked();
    }
  }

  sizeMenu.append(MenuSeparator());
  sizeMenu.append(MenuItem().setIcon(Icon::Action::Remove).setText("Shrink Window To Size").onActivate([&] {
    resizeWindow();
  }));
  sizeMenu.append(MenuItem().setIcon(Icon::Place::Settings).setText("Center Window").onActivate([&] {
    setCentered();
  }));
}

auto Presentation::configureViewport() -> void {
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video) video->configure(width, height, 60, 60);
}

auto Presentation::clearViewport() -> void {
  if(!emulator || !emulator->loaded()) viewportLayout.setPadding();
  if(!visible() || !video) return;

  uint32_t* output;
  uint length = 0;
  uint width = 16;
  uint height = 16;
  if(video->acquire(output, length, width, height)) {
    for(uint y : range(height)) {
      auto line = output + y * (length >> 2);
      for(uint x : range(width)) *line++ = 0xff000000;
    }
    video->release();
    video->output();
  }
}

auto Presentation::resizeViewport() -> void {
  uint layoutWidth = viewportLayout.geometry().width();
  uint layoutHeight = viewportLayout.geometry().height();

  uint width = 320;
  uint height = 240;

  if(emulator) {
    auto display = emulator->displays().first();
    width = display.width;
    height = display.height;
    if(settings["View/AspectCorrection"].boolean()) width *= display.aspectCorrection;
    if(!settings["View/Overscan"].boolean()) {
      if(display.type == Emulator::Interface::Display::Type::CRT) {
        uint overscanHorizontal = settings["View/Overscan/Horizontal"].natural();
        uint overscanVertical = settings["View/Overscan/Vertical"].natural();
        width -= overscanHorizontal * 2;
        height -= overscanVertical * 2;
      }
    }
  }

  if(visible() && !fullScreen()) {
    uint widthMultiplier = layoutWidth / width;
    uint heightMultiplier = layoutHeight / height;
    uint multiplier = max(1, min(widthMultiplier, heightMultiplier));
    settings["View/Multiplier"].setValue(multiplier);
    for(auto item : sizeGroup.objects<MenuRadioItem>()) {
      if(auto property = item.property("multiplier")) {
        if(property.natural() == multiplier) item.setChecked();
      }
    }
  }

  if(!emulator || !emulator->loaded()) return clearViewport();
  if(!video) return;

  uint viewportWidth;
  uint viewportHeight;
  if(settings["View/Output"].text() == "Center") {
    uint widthMultiplier = layoutWidth / width;
    uint heightMultiplier = layoutHeight / height;
    uint multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  } else if(settings["View/Output"].text() == "Scale") {
    double widthMultiplier = (double)layoutWidth / width;
    double heightMultiplier = (double)layoutHeight / height;
    double multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  } else if(settings["View/Output"].text() == "Stretch" || 1) {
    viewportWidth = layoutWidth;
    viewportHeight = layoutHeight;
  }

  //center viewport within viewportLayout by use of viewportLayout padding
  uint paddingWidth = layoutWidth - viewportWidth;
  uint paddingHeight = layoutHeight - viewportHeight;
  viewportLayout.setPadding({
    paddingWidth / 2, paddingHeight / 2,
    paddingWidth - paddingWidth / 2, paddingHeight - paddingHeight / 2
  });
}

auto Presentation::resizeWindow() -> void {
  if(fullScreen()) return;
  if(maximized()) setMaximized(false);

  uint width = 320;
  uint height = 240;
  uint multiplier = max(1, settings["View/Multiplier"].natural());
  uint statusHeight = settings["View/StatusBar"].boolean() ? StatusHeight : 0;

  if(emulator) {
    auto display = emulator->displays().first();
    width = display.width;
    height = display.height;
    if(settings["View/AspectCorrection"].boolean()) width *= display.aspectCorrection;
    if(!settings["View/Overscan"].boolean()) {
      if(display.type == Emulator::Interface::Display::Type::CRT) {
        uint overscanHorizontal = settings["View/Overscan/Horizontal"].natural();
        uint overscanVertical = settings["View/Overscan/Vertical"].natural();
        width -= overscanHorizontal * 2;
        height -= overscanVertical * 2;
      }
    }
  }

  setMinimumSize({width, height + statusHeight});
  setSize({width * multiplier, height * multiplier + statusHeight});
  layout.setGeometry(layout.geometry());
  resizeViewport();
}

auto Presentation::toggleFullScreen() -> void {
  if(!fullScreen()) {
    if(settings["View/StatusBar"].boolean()) {
      layout.remove(statusLayout);
    }
    menuBar.setVisible(false);
    setFullScreen(true);
    video->setExclusive(settings["Video/Exclusive"].boolean());
    if(video->exclusive()) setVisible(false);
    if(!input->acquired()) input->acquire();
    resizeViewport();
  } else {
    if(input->acquired()) input->release();
    if(video->exclusive()) setVisible(true);
    video->setExclusive(false);
    setFullScreen(false);
    menuBar.setVisible(true);
    if(settings["View/StatusBar"].boolean()) {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    resizeWindow();
    setCentered();
  }
}

auto Presentation::loadSystems() -> void {
  systemsMenu.reset();
  for(auto system : settings.find("Systems/System")) {
    if(!system["Visible"].boolean()) continue;
    MenuItem item{&systemsMenu};
    string name = system.text();
    string filename = system["Load"].text();
    string load = Location::base(filename).trimRight("/", 1L);
    string alias = system["Alias"].text();
    item.setIcon(load ? (image)Icon::Emblem::Folder : (image)Icon::Device::Storage);
    item.setText({alias ? alias : load ? load : name, " ..."});
    item.onActivate([=] {
      for(auto& emulator : program->emulators) {
        auto information = emulator->information();
        if(name == information.name) {
          if(filename) program->gameQueue.append(filename);
          program->load(*emulator);
          break;
        }
      }
    });
  }

  //add icarus menu option -- but only if icarus binary is present
  if(execute("icarus", "--name").output.strip() == "icarus") {
    if(systemsMenu.actionCount()) systemsMenu.append(MenuSeparator());
    MenuItem item{&systemsMenu};
    item.setIcon(Icon::Emblem::File);
    item.setText("Load ROM File ...");
    item.onActivate([&] {
      audio->clear();
      if(auto location = execute("icarus", "--import")) {
        program->gameQueue.append(location.output.strip());
        program->load();
      }
    });
  }
}

auto Presentation::loadShaders() -> void {
  auto pathname = locate("shaders/");

  if(settings["Video/Driver"].text() == "OpenGL 3.2") {
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
