#include "../bsnes.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemMenu.setText("System");
  loadGame.setText("Load Game ...").onActivate([&] {
    program->load();
  });
  loadRecentGame.setText("Load Recent Game");
  updateRecentGames();
  resetSystem.setText("Reset System").setEnabled(false).onActivate([&] {
    if(emulator->loaded()) emulator->reset();
  });
  unloadGame.setText("Unload Game").setEnabled(false).onActivate([&] {
    program->unload();
  });
  controllerPort1.setText("Controller Port 1");
  controllerPort2.setText("Controller Port 2");
  expansionPort.setText("Expansion Port");
  for(auto& port : emulator->ports) {
    Menu* menu = nullptr;
    if(port.name == "Controller Port 1") menu = &controllerPort1;
    if(port.name == "Controller Port 2") menu = &controllerPort2;
    if(port.name == "Expansion Port") menu = &expansionPort;
    if(!menu) continue;

    Group devices;
    for(auto& device : port.devices) {
      if(port.name != "Expansion Port" && device.name == "None") continue;
      if(port.name == "Expansion Port" && device.name == "21fx") continue;
      MenuRadioItem item{menu};
      item.setText(device.name).onActivate([=] {
        auto path = string{"Emulator/", port.name}.replace(" ", "");
        settings(path).setValue(device.name);
        emulator->connect(port.id, device.id);
      });
      devices.append(item);
    }
    auto path = string{"Emulator/", port.name}.replace(" ", "");
    auto device = settings(path).text();
    bool found = false;
    if(devices.objectCount() > 1) {
      for(auto item : devices.objects<MenuRadioItem>()) {
        if(item.text() == device) item.setChecked(), found = true;
      }
    }
    //select the first device when a new settings file is being created
    if(devices.objectCount() && !found) {
      devices.objects<MenuRadioItem>()(0).doActivate();
    }
  }
  quit.setText("Quit").onActivate([&] { program->quit(); });

  settingsMenu.setText("Settings");
  viewMenu.setText("View");
  smallView.setText("Small").onActivate([&] {
    settings["View/Size"].setValue("Small");
    resizeWindow();
  });
  mediumView.setText("Medium").onActivate([&] {
    settings["View/Size"].setValue("Medium");
    resizeWindow();
  });
  largeView.setText("Large").onActivate([&] {
    settings["View/Size"].setValue("Large");
    resizeWindow();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(settings["View/AspectCorrection"].boolean()).onToggle([&] {
    settings["View/AspectCorrection"].setValue(aspectCorrection.checked());
    resizeWindow();
  });
  overscanCropping.setText("Overscan Cropping").setChecked(settings["View/OverscanCropping"].boolean()).onToggle([&] {
    settings["View/OverscanCropping"].setValue(overscanCropping.checked());
    resizeWindow();
  });
  integralScaling.setText("Integral Scaling").setChecked(settings["View/IntegralScaling"].boolean()).onToggle([&] {
    settings["View/IntegralScaling"].setValue(integralScaling.checked());
    resizeViewport();
  });
  blurEmulation.setText("Blur Emulation").setChecked(settings["View/BlurEmulation"].boolean()).onToggle([&] {
    settings["View/BlurEmulation"].setValue(blurEmulation.checked());
    emulator->set("Blur Emulation", blurEmulation.checked());
  }).doToggle();
  colorEmulation.setText("Color Emulation").setChecked(settings["View/ColorEmulation"].boolean()).onToggle([&] {
    settings["View/ColorEmulation"].setValue(colorEmulation.checked());
    emulator->set("Color Emulation", colorEmulation.checked());
  }).doToggle();
  shaderMenu.setText("Shader");
  updateShaders();
  muteAudio.setText("Mute Audio").setChecked(settings["Audio/Mute"].boolean()).onToggle([&] {
    settings["Audio/Mute"].setValue(muteAudio.checked());
  });
  showStatusBar.setText("Show Status Bar").setChecked(settings["UserInterface/ShowStatusBar"].boolean()).onToggle([&] {
    settings["UserInterface/ShowStatusBar"].setValue(showStatusBar.checked());
    statusBar.setVisible(showStatusBar.checked());
    if(visible()) resizeWindow();
  });
  inputSettings.setText("Input ...").onActivate([&] { settingsWindow->show(0); });
  hotkeySettings.setText("Hotkeys ...").onActivate([&] { settingsWindow->show(1); });
  pathSettings.setText("Paths ...").onActivate([&] { settingsWindow->show(2); });
  advancedSettings.setText("Advanced ...").onActivate([&] { settingsWindow->show(3); });

  toolsMenu.setText("Tools").setVisible(false);
  saveState.setText("Save State");
  saveState1.setText("Slot 1").onActivate([&] { program->saveState(1); });
  saveState2.setText("Slot 2").onActivate([&] { program->saveState(2); });
  saveState3.setText("Slot 3").onActivate([&] { program->saveState(3); });
  saveState4.setText("Slot 4").onActivate([&] { program->saveState(4); });
  saveState5.setText("Slot 5").onActivate([&] { program->saveState(5); });
  loadState.setText("Load State");
  loadState1.setText("Slot 1").onActivate([&] { program->loadState(1); });
  loadState2.setText("Slot 2").onActivate([&] { program->loadState(2); });
  loadState3.setText("Slot 3").onActivate([&] { program->loadState(3); });
  loadState4.setText("Slot 4").onActivate([&] { program->loadState(4); });
  loadState5.setText("Slot 5").onActivate([&] { program->loadState(5); });
  pauseEmulation.setText("Pause Emulation").onToggle([&] {
    if(pauseEmulation.checked()) audio->clear();
  });
  cheatEditor.setText("Cheat Editor ...").onActivate([&] { toolsWindow->show(0); });

  helpMenu.setText("Help");
  about.setText("About ...").onActivate([&] {
    aboutWindow->setCentered(*this).setVisible().setFocused();
  });

  viewport.setDroppable().onDrop([&](auto locations) {
    program->gameQueue = locations;
    program->load();
    presentation->setFocused();
  });

  statusBar.setFont(Font().setBold());
  statusBar.setVisible(settings["UserInterface/ShowStatusBar"].boolean());

  onClose([&] {
    program->quit();
  });

  onSize([&] {
    resizeViewport();
  });

  setTitle({"bsnes v", Emulator::Version});
  setBackgroundColor({0, 0, 0});
  resizeWindow();
  setCentered();

  #if defined(PLATFORM_WINDOWS)
  Application::Windows::onModalChange([&](bool modal) {
    if(modal && audio) audio->clear();
  });
  Application::Windows::onScreenSaver([&]() -> bool {
    if(emulator->loaded()) {
      if(pauseEmulation.checked()) return true;
      if(!program->focused() && settingsWindow->input.pauseEmulation.checked()) return true;
      return false;
    }
    return true;
  });
  #endif

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] { about.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onPreferences([&] { settingsWindow->show(0); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::drawIcon(uint32_t* output, uint length, uint width, uint height) -> void {
  int ox = width  - 144;
  int oy = height - 128;
  if(ox >= 0 && oy >= 0) {
    image icon{Resource::Icon};
    icon.alphaBlend(0xff000000);
    for(uint y : range(128)) {
      auto target = output + (y + oy) * (length >> 2) + ox;
      auto source = (uint32_t*)icon.data() + y * 128;
      memory::copy<uint32_t>(target, source, 128);
    }
  }
}

auto Presentation::clearViewport() -> void {
  if(!video) return;

  if(!emulator->loaded()) {
    viewport.setGeometry({0, 0, geometry().width(), geometry().height()});
  }

  uint32_t* output;
  uint length;
  uint width = viewport.geometry().width();
  uint height = viewport.geometry().height();
  if(video->lock(output, length, width, height)) {
    for(uint y : range(height)) {
      auto line = output + y * (length >> 2);
      for(uint x : range(width)) *line++ = 0xff000000;
    }
    if(!emulator->loaded()) drawIcon(output, length, width, height);
    video->unlock();
    video->output();
  }
}

auto Presentation::resizeViewport() -> void {
  uint windowWidth = geometry().width();
  uint windowHeight = geometry().height();

  if(!emulator->loaded()) {
    viewport.setGeometry({0, 0, windowWidth, windowHeight});
    return clearViewport();
  }

  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 223.0 : 239.0);

  if(settings["View/IntegralScaling"].boolean()) {
    uint widthMultiplier = windowWidth / width;
    uint heightMultiplier = windowHeight / height;
    uint multiplier = min(widthMultiplier, heightMultiplier);
    uint viewportWidth = width * multiplier;
    uint viewportHeight = height * multiplier;
    viewport.setGeometry({
      (windowWidth - viewportWidth) / 2, (windowHeight - viewportHeight) / 2,
      viewportWidth, viewportHeight
    });
  } else {
    double widthMultiplier = (double)windowWidth / width;
    double heightMultiplier = (double)windowHeight / height;
    double multiplier = min(widthMultiplier, heightMultiplier);
    uint viewportWidth = width * multiplier;
    uint viewportHeight = height * multiplier;
    viewport.setGeometry({
      (windowWidth - viewportWidth) / 2, (windowHeight - viewportHeight) / 2,
      viewportWidth, viewportHeight
    });
  }

  clearViewport();
}

auto Presentation::resizeWindow() -> void {
  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 223.0 : 239.0);

  uint multiplier = 2;
  if(settings["View/Size"].text() == "Small" ) multiplier = 2;
  if(settings["View/Size"].text() == "Medium") multiplier = 3;
  if(settings["View/Size"].text() == "Large" ) multiplier = 4;

  setSize({width * multiplier, height * multiplier});
  resizeViewport();
}

auto Presentation::toggleFullscreenMode() -> void {
  if(!fullScreen()) {
    statusBar.setVisible(false);
    menuBar.setVisible(false);
    setFullScreen(true);
    video->setExclusive(settings["Video/Exclusive"].boolean());
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

auto Presentation::updateRecentGames() -> void {
  loadRecentGame.reset();
  for(auto index : range(5)) {
    MenuItem item;
    if(auto game = settings[string{"Game/Recent/", 1 + index}].text()) {
      string displayName;
      auto games = game.split("|");
      for(auto& part : games) {
        displayName.append(Location::prefix(part), " + ");
      }
      displayName.trimRight(" + ", 1L);
      item.setText(displayName).onActivate([=] {
        program->gameQueue = games;
        program->load();
      });
    } else {
      item.setText("<empty>").setEnabled(false);
    }
    loadRecentGame.append(item);
  }
  loadRecentGame.append(MenuSeparator());
  loadRecentGame.append(MenuItem().setText("Clear List").onActivate([&] {
    settings("Game/Recent/1").setValue("");
    settings("Game/Recent/2").setValue("");
    settings("Game/Recent/3").setValue("");
    settings("Game/Recent/4").setValue("");
    settings("Game/Recent/5").setValue("");
    updateRecentGames();
  }));
}

auto Presentation::addRecentGame(string location) -> void {
  auto game1 = settings["Game/Recent/1"].text();
  auto game2 = settings["Game/Recent/2"].text();
  auto game3 = settings["Game/Recent/3"].text();
  auto game4 = settings["Game/Recent/4"].text();
  auto game5 = settings["Game/Recent/5"].text();

         if(game1 == location) {
    game1 = location;
  } else if(game2 == location) {
    game2 = game1;
    game1 = location;
  } else if(game3 == location) {
    game3 = game2;
    game2 = game1;
    game1 = location;
  } else if(game4 == location) {
    game4 = game3;
    game3 = game2;
    game2 = game1;
    game1 = location;
  } else {
    game5 = game4;
    game4 = game3;
    game3 = game2;
    game2 = game1;
    game1 = location;
  }

  settings("Game/Recent/1").setValue(game1);
  settings("Game/Recent/2").setValue(game2);
  settings("Game/Recent/3").setValue(game3);
  settings("Game/Recent/4").setValue(game4);
  settings("Game/Recent/5").setValue(game5);
  updateRecentGames();
}

auto Presentation::updateShaders() -> void {
  shaderMenu.reset();

  Group shaders;

  MenuRadioItem none{&shaderMenu};
  none.setText("None").onActivate([&] {
    settings["Video/Shader"].setValue("None");
    program->updateVideoShader();
  });
  shaders.append(none);

  MenuRadioItem blur{&shaderMenu};
  blur.setText("Blur").onActivate([&] {
    settings["Video/Shader"].setValue("Blur");
    program->updateVideoShader();
  });
  shaders.append(blur);

  auto location = locate("shaders/");

  if(settings["Video/Driver"].text() == "OpenGL") {
    for(auto shader : directory::folders(location, "*.shader")) {
      if(shaders.objectCount() == 2) shaderMenu.append(MenuSeparator());
      MenuRadioItem item{&shaderMenu};
      item.setText(string{shader}.trimRight(".shader", 1L)).onActivate([=] {
        settings["Video/Shader"].setValue({location, shader});
        program->updateVideoShader();
      });
      shaders.append(item);
    }
  }

  if(settings["Video/Shader"].text() == "None") none.setChecked();
  if(settings["Video/Shader"].text() == "Blur") blur.setChecked();
  for(auto item : shaders.objects<MenuRadioItem>()) {
    if(settings["Video/Shader"].text() == string{location, item.text(), ".shader/"}) {
      item.setChecked();
    }
  }
}
