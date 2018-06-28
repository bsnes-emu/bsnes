#include "../bsnes.hpp"
#include "about.cpp"
unique_pointer<AboutWindow> aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemMenu.setText("System");
  loadGame.setIcon(Icon::Action::Open).setText("Load Game ...").onActivate([&] {
    program->load();
  });
  loadRecentGame.setIcon(Icon::Action::Open).setText("Load Recent Game");
  updateRecentGames();
  resetSystem.setIcon(Icon::Action::Refresh).setText("Reset System").setEnabled(false).onActivate([&] {
    program->reset();
  });
  unloadGame.setIcon(Icon::Action::Remove).setText("Unload Game").setEnabled(false).onActivate([&] {
    program->unload();
  });
  controllerPort1.setIcon(Icon::Device::Joypad).setText("Controller Port 1");
  controllerPort2.setIcon(Icon::Device::Joypad).setText("Controller Port 2");
  expansionPort.setIcon(Icon::Device::Storage).setText("Expansion Port");
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
  quit.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { program->quit(); });

  settingsMenu.setText("Settings");
  scaleMenu.setIcon(Icon::Emblem::Image).setText("View");
  smallestScale.setText("Smallest (240p)").onActivate([&] {
    settings["View/Size"].setValue("Smallest");
    resizeWindow();
  });
  smallScale.setText("Small (480p)").onActivate([&] {
    settings["View/Size"].setValue("Small");
    resizeWindow();
  });
  mediumScale.setText("Medium (720p)").onActivate([&] {
    settings["View/Size"].setValue("Medium");
    resizeWindow();
  });
  largeScale.setText("Large (960p)").onActivate([&] {
    settings["View/Size"].setValue("Large");
    resizeWindow();
  });
  largestScale.setText("Largest (1200p)").onActivate([&] {
    settings["View/Size"].setValue("Largest");
    resizeWindow();
  });
  outputMenu.setIcon(Icon::Emblem::Image).setText("Output");
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
  shaderMenu.setIcon(Icon::Emblem::Image).setText("Shader");
  updateShaders();
  synchronizeVideo.setText("Synchronize Video").setChecked(settings["Video/Blocking"].boolean()).onToggle([&] {
    settings["Video/Blocking"].setValue(synchronizeVideo.checked());
    program->updateVideoBlocking();
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(settings["Audio/Blocking"].boolean()).onToggle([&] {
    settings["Audio/Blocking"].setValue(synchronizeAudio.checked());
    program->updateAudioBlocking();
  });
  muteAudio.setText("Mute Audio").setChecked(settings["Audio/Mute"].boolean()).onToggle([&] {
    settings["Audio/Mute"].setValue(muteAudio.checked());
    program->updateAudioEffects();
  });
  showStatusBar.setText("Show Status Bar").setChecked(settings["UserInterface/ShowStatusBar"].boolean()).onToggle([&] {
    settings["UserInterface/ShowStatusBar"].setValue(showStatusBar.checked());
    if(!showStatusBar.checked()) {
      layout.remove(statusLayout);
    } else {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    if(visible()) resizeWindow();
  });
  videoSettings.setIcon(Icon::Device::Display).setText("Video ...").onActivate([&] { settingsWindow->show(0); });
  audioSettings.setIcon(Icon::Device::Speaker).setText("Audio ...").onActivate([&] { settingsWindow->show(1); });
  inputSettings.setIcon(Icon::Device::Joypad).setText("Input ...").onActivate([&] { settingsWindow->show(2); });
  hotkeySettings.setIcon(Icon::Device::Keyboard).setText("Hotkeys ...").onActivate([&] { settingsWindow->show(3); });
  pathSettings.setIcon(Icon::Emblem::Folder).setText("Paths ...").onActivate([&] { settingsWindow->show(4); });
  advancedSettings.setIcon(Icon::Action::Settings).setText("Advanced ...").onActivate([&] { settingsWindow->show(5); });

  toolsMenu.setText("Tools").setVisible(false);
  saveState.setIcon(Icon::Action::Save).setText("Save State");
  for(uint index : range(QuickStates)) {
    saveState.append(MenuItem().setText({"Slot ", 1 + index}).onActivate([=] {
      program->saveState({"quick/slot ", 1 + index});
    }));
  }
  loadState.setIcon(Icon::Media::Play).setText("Load State");
  for(uint index : range(QuickStates)) {
    loadState.append(MenuItem().setText({"Slot ", 1 + index}).onActivate([=] {
      program->loadState({"quick/slot ", 1 + index});
    }));
  }
  loadState.append(MenuSeparator());
  loadState.append(MenuItem().setIcon(Icon::Edit::Undo).setText("Undo Last Save").onActivate([&] {
    program->loadState("quick/recovery");
  }));
  speedMenu.setIcon(Icon::Device::Clock).setText("Speed");
  speedSlowest.setText("Slowest (50%)").setProperty("multiplier", "2.0").onActivate([&] { program->updateAudioFrequency(); });
  speedSlow.setText("Slow (75%)").setProperty("multiplier", "1.333").onActivate([&] { program->updateAudioFrequency(); });
  speedNormal.setText("Normal (100%)").setProperty("multiplier", "1.0").onActivate([&] { program->updateAudioFrequency(); });
  speedFast.setText("Fast (150%)").setProperty("multiplier", "0.667").onActivate([&] { program->updateAudioFrequency(); });
  speedFastest.setText("Fastest (200%)").setProperty("multiplier", "0.5").onActivate([&] { program->updateAudioFrequency(); });
  pauseEmulation.setText("Pause Emulation").onToggle([&] {
    if(pauseEmulation.checked()) audio->clear();
  });
  cheatEditor.setIcon(Icon::Edit::Replace).setText("Cheat Editor ...").onActivate([&] { toolsWindow->show(0); });
  stateManager.setIcon(Icon::Application::FileManager).setText("State Manager ...").onActivate([&] { toolsWindow->show(1); });

  helpMenu.setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/bsnes/");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    aboutWindow->setCentered(*this).setVisible().setFocused();
  });

  viewport.setDroppable().onDrop([&](string_vector locations) {
    program->gameQueue = locations;
    program->load();
    setFocused();
  });

  if(!settings["UserInterface/ShowStatusBar"].boolean()) {
    layout.remove(statusLayout);
  }

  auto font = Font().setBold();
  auto back = Color{ 32,  32,  32};
  auto fore = Color{255, 255, 255};

  updateStatusIcon();

  spacerIcon.setBackgroundColor(back).setForegroundColor(fore);

  spacerLeft.setBackgroundColor(back).setForegroundColor(fore);

  statusLeft.setFont(font);
  statusLeft.setAlignment(0.0);
  statusLeft.setBackgroundColor(back);
  statusLeft.setForegroundColor(fore);

  statusRight.setFont(font);
  statusRight.setAlignment(1.0);
  statusRight.setBackgroundColor(back);
  statusRight.setForegroundColor(fore);

  spacerRight.setBackgroundColor(back).setForegroundColor(fore);

  program->updateStatus();

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
  Application::Cocoa::onPreferences([&] { settingsWindow->show(2); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::updateStatusIcon() -> void {
  image icon;
  icon.allocate(16, StatusHeight);
  icon.fill(0xff202020);

  if(emulator->loaded()) {
    image emblem{program->verified() ? Icon::Emblem::Program : Icon::Emblem::Binary};
    icon.impose(image::blend::sourceAlpha, 0, (StatusHeight - 16) / 2, emblem, 0, 0, 16, 16);
  }

  statusIcon.setIcon(icon);
}

auto Presentation::drawIcon(uint32_t* output, uint length, uint width, uint height) -> void {
  return;

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
    viewportLayout.setPadding();
  }

  uint32_t* output;
  uint length;
  uint width = 16;
  uint height = 16;
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
  if(!emulator->loaded()) return clearViewport();

  uint windowWidth = viewportLayout.geometry().width();
  uint windowHeight = viewportLayout.geometry().height();

  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 224.0 : 240.0);
  uint viewportWidth, viewportHeight;

  if(settings["View/IntegralScaling"].boolean()) {
    uint widthMultiplier = windowWidth / width;
    uint heightMultiplier = windowHeight / height;
    uint multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  } else {
    double widthMultiplier = (double)windowWidth / width;
    double heightMultiplier = (double)windowHeight / height;
    double multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  }

  //center viewport within viewportLayout by use of viewportLayout padding
  uint paddingWidth = windowWidth - viewportWidth;
  uint paddingHeight = windowHeight - viewportHeight;
  viewportLayout.setPadding({
    paddingWidth / 2, paddingHeight / 2,
    paddingWidth - paddingWidth / 2, paddingHeight - paddingHeight / 2
  });

  clearViewport();
}

auto Presentation::resizeWindow() -> void {
  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 224.0 : 240.0);
  uint statusHeight = settings["UserInterface/ShowStatusBar"].boolean() ? StatusHeight : 0;

  uint multiplier = 2;
  if(settings["View/Size"].text() == "Smallest") multiplier = 1;
  if(settings["View/Size"].text() == "Small"   ) multiplier = 2;
  if(settings["View/Size"].text() == "Medium"  ) multiplier = 3;
  if(settings["View/Size"].text() == "Large"   ) multiplier = 4;
  if(settings["View/Size"].text() == "Largest" ) multiplier = 5;

  setSize({width * multiplier, height * multiplier + statusHeight});
  resizeViewport();
}

auto Presentation::toggleFullscreenMode() -> void {
  if(!fullScreen()) {
    if(settings["UserInterface/ShowStatusBar"].boolean()) {
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
    if(settings["UserInterface/ShowStatusBar"].boolean()) {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    resizeWindow();
    setCentered();
  }
}

auto Presentation::updateRecentGames() -> void {
  loadRecentGame.reset();
  for(auto index : range(RecentGames)) {
    MenuItem item;
    if(auto game = settings[string{"Game/Recent/", 1 + index}].text()) {
      string displayName;
      auto games = game.split("|");
      for(auto& part : games) {
        displayName.append(Location::prefix(part), " + ");
      }
      displayName.trimRight(" + ", 1L);
      item.setIcon(games(0).endsWith("/") ? Icon::Action::Open : Icon::Emblem::File);
      item.setText(displayName);
      item.onActivate([=] {
        program->gameQueue = games;
        program->load();
      });
    } else {
      item.setText("<empty>");
      item.setEnabled(false);
    }
    loadRecentGame.append(item);
  }
  loadRecentGame.append(MenuSeparator());
  loadRecentGame.append(MenuItem().setIcon(Icon::Edit::Clear).setText("Clear List").onActivate([&] {
    for(auto index : range(RecentGames)) {
      settings({"Game/Recent/", 1 + index}).setValue("");
    }
    updateRecentGames();
  }));
}

auto Presentation::addRecentGame(string location) -> void {
  for(uint index : range(RecentGames + 1)) {
    auto value = settings[{"Game/Recent/", 1 + index}].text();
    if(!value || value == location) {
      for(uint n : rrange(index + 1)) {
        if(1 + n > RecentGames) continue;
        settings({"Game/Recent/", 1 + n}).setValue(settings[{"Game/Recent/", n}].text());
      }
      break;
    }
  }
  settings("Game/Recent/1").setValue(location);
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
