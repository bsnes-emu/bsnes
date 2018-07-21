#include "../higan.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemsMenu.setText("Systems");

  systemMenu.setVisible(false);

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

  statusBar.setFont(Font().setBold());
  statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());

  image icon{Resource::Icon};
  icon.alphaBlend(0xff000000);
  canvas.setIcon(icon).setVisible(false);

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
  Application::Windows::onModalChange([&](bool modal) {
    if(modal && audio) audio->clear();
  });
  Application::Windows::onScreenSaver([&]() -> bool {
    if(emulator && emulator->loaded()) {
      if(program->pause) return true;
      if(!program->focused() && settingsManager->input.pauseEmulation.checked()) return true;
      return false;
    }
    return true;
  });
  #endif

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

  systemMenu.reset();
  for(auto& port : emulator->ports) {
    Menu menu{&systemMenu};
    menu.setProperty("portID", port.id);
    menu.setText(port.name);
    if(port.name.beginsWith("Expansion") || port.name.beginsWith("Extension")) {
      menu.setIcon(Icon::Device::Storage);
    } else {
      menu.setIcon(Icon::Device::Joypad);
    }

    auto path = string{emulator->information.name, "/", port.name}.replace(" ", "");
    auto deviceName = settings(path).text();
    auto deviceID = emulator->connected(port.id);

    Group devices;
    for(auto& device : port.devices) {
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

  if(emulator->information.resettable) {
    systemMenu.append(MenuItem().setText("Soft Reset").setIcon(Icon::Action::Refresh).onActivate([&] {
      program->softReset();
    }));
  }

  systemMenu.append(MenuItem().setText("Power Cycle").setIcon(Icon::Action::Refresh).onActivate([&] {
    program->powerCycle();
  }));

  systemMenu.append(MenuItem().setText("Unload").setIcon(Icon::Media::Eject).onActivate([&] {
    program->unloadMedium();
  }));

  updateEmulatorDeviceSelections();
}

auto Presentation::updateEmulatorDeviceSelections() -> void {
  if(!emulator) return;

  for(auto& port : emulator->ports) {
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

auto Presentation::drawIcon(uint32_t* output, uint length, uint width, uint height) -> void {
  return;

  int ox = width  - 128;
  int oy = height - 128;
  if(ox >= 0 && oy >= 0) {
    image icon{Resource::Icon};
    icon.alphaBlend(0xff000000);
    for(uint y : range(112)) {
      auto target = output + (y + oy) * (length >> 2) + ox;
      auto source = (uint32_t*)icon.data() + y * 112;
      memory::copy<uint32_t>(target, source, 112);
    }
  }
}

auto Presentation::clearViewport() -> void {
  if(!video) return;

  if(!emulator || !emulator->loaded()) {
    viewport.setGeometry({0, 0, geometry().width(), geometry().height()});
  }

  uint32_t* output;
  uint length = 0;
  uint width = 16;
  uint height = 16;
  if(video->lock(output, length, width, height)) {
    for(uint y : range(height)) {
      auto line = output + y * (length >> 2);
      for(uint x : range(width)) *line++ = 0xff000000;
    }
    if(!emulator || !emulator->loaded()) drawIcon(output, length, width, height);
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
    auto information = emulator->videoInformation();
    emulatorWidth = information.width;
    emulatorHeight = information.height;
    aspectCorrection = information.aspectCorrection;
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

  if(emulator && emulator->loaded()) {
    viewport.setGeometry({
      (viewportWidth - emulatorWidth) / 2, (viewportHeight - emulatorHeight) / 2,
      emulatorWidth, emulatorHeight
    });
  } else {
    viewport.setGeometry({0, 0, geometry().width(), geometry().height()});
  }

  //clear video area again to ensure entire viewport area has been painted in
  clearViewport();
}

auto Presentation::toggleFullScreen() -> void {
  if(!fullScreen()) {
    statusBar.setVisible(false);
    menuBar.setVisible(false);
    setFullScreen(true);
    video->setExclusive(settings["Video/Fullscreen/Exclusive"].boolean());
    if(video->exclusive()) setVisible(false);
    if(!input->acquired()) input->acquire();
  } else {
    if(input->acquired()) input->release();
    if(video->exclusive()) setVisible(true);
    video->setExclusive(false);
    setFullScreen(false);
    menuBar.setVisible(true);
    statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());
  }
  //hack: give window geometry time to update after toggling fullscreen and menu/status bars
  usleep(20 * 1000);
  Application::processEvents();
  resizeViewport();
}

auto Presentation::loadSystems() -> void {
  systemsMenu.reset();
  for(auto system : settings.find("Systems/System")) {
    if(!system["Visible"].boolean()) continue;
    MenuItem item;
    string name = system.text();
    string filename = system["Load"].text();
    string load = Location::base(filename).trimRight("/", 1L);
    string alias = system["Alias"].text();
    item
    .setIcon(load ? Icon::Emblem::Folder : Icon::Device::Storage)
    .setText({alias ? alias : load ? load : name, " ..."}).onActivate([=] {
      for(auto& emulator : program->emulators) {
        if(name == emulator->information.name) {
          if(filename) program->mediumQueue.append(filename);
          program->loadMedium(*emulator, emulator->media(0));
          break;
        }
      }
    });
    systemsMenu.append(item);
  }

  //add icarus menu option -- but only if icarus binary is present
  if(execute("icarus", "--name").output.strip() == "icarus") {
    if(systemsMenu.actionCount()) systemsMenu.append(MenuSeparator());
    systemsMenu.append(MenuItem()
    .setIcon(Icon::Emblem::File)
    .setText("Load ROM File ...").onActivate([&] {
      audio->clear();
      if(auto location = execute("icarus", "--import")) {
        program->mediumQueue.append(location.output.strip());
        program->loadMedium();
      }
    }));
  }
}

auto Presentation::loadShaders() -> void {
  auto pathname = locate("shaders/");

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
