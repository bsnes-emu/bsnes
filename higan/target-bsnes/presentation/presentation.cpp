#include "../bsnes.hpp"
#include "about.cpp"
AboutWindow aboutWindow;
unique_pointer<Presentation> presentation;

Presentation::Presentation() {
  presentation = this;

  systemMenu.setText(tr("System"));
  loadGame.setIcon(Icon::Action::Open).setText({tr("Load Game"), " ..."}).onActivate([&] {
    program->load();
  });
  loadRecentGame.setIcon(Icon::Action::Open).setText(tr("Load Recent Game"));
  updateRecentGames();
  resetSystem.setIcon(Icon::Action::Refresh).setText(tr("Reset System")).setEnabled(false).onActivate([&] {
    program->reset();
  });
  unloadGame.setIcon(Icon::Action::Remove).setText(tr("Unload Game")).setEnabled(false).onActivate([&] {
    program->unload();
  });
  controllerPort1.setIcon(Icon::Device::Joypad).setText(tr("Controller Port 1"));
  controllerPort2.setIcon(Icon::Device::Joypad).setText(tr("Controller Port 2"));
  expansionPort.setIcon(Icon::Device::Storage).setText(tr("Expansion Port"));
  updateDeviceMenu();
  quit.setIcon(Icon::Action::Quit).setText(tr("Quit")).onActivate([&] { program->quit(); });

  settingsMenu.setText(tr("Settings"));
  sizeMenu.setIcon(Icon::Emblem::Image).setText("Size");
  updateSizeMenu();
  outputMenu.setIcon(Icon::Emblem::Image).setText("Output");
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
  aspectCorrection.setText("Aspect Correction").setChecked(settings["View/AspectCorrection"].boolean()).onToggle([&] {
    settings["View/AspectCorrection"].setValue(aspectCorrection.checked());
    resizeWindow();
  });
  overscanCropping.setText("Overscan Cropping").setChecked(settings["View/OverscanCropping"].boolean()).onToggle([&] {
    settings["View/OverscanCropping"].setValue(overscanCropping.checked());
    resizeWindow();
  });
  blurEmulation.setText("Blur Emulation").setChecked(settings["View/BlurEmulation"].boolean()).onToggle([&] {
    settings["View/BlurEmulation"].setValue(blurEmulation.checked());
    emulator->configure("video/blurEmulation", blurEmulation.checked());
  }).doToggle();
  shaderMenu.setIcon(Icon::Emblem::Image).setText("Shader");
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
  configurationSettings.setIcon(Icon::Action::Settings).setText("Configuration ...").onActivate([&] { settingsWindow->show(5); });
  driverSettings.setIcon(Icon::Place::Settings).setText("Drivers ...").onActivate([&] { settingsWindow->show(6); });

  toolsMenu.setText(tr("Tools")).setVisible(false);
  saveState.setIcon(Icon::Action::Save).setText("Save State");
  for(uint index : range(QuickStates)) {
    MenuItem item{&saveState};
    item.setProperty("name", {"quick/slot ", 1 + index});
    item.setProperty("title", {"Slot ", 1 + index});
    item.setText({"Slot ", 1 + index});
    item.onActivate([=] { program->saveState({"quick/slot ", 1 + index}); });
  }
  loadState.setIcon(Icon::Media::Play).setText("Load State");
  for(uint index : range(QuickStates)) {
    MenuItem item{&loadState};
    item.setProperty("name", {"quick/slot ", 1 + index});
    item.setProperty("title", {"Slot ", 1 + index});
    item.setText({"Slot ", 1 + index});
    item.onActivate([=] { program->loadState({"quick/slot ", 1 + index}); });
  }
  loadState.append(MenuSeparator());
  loadState.append(MenuItem()
  .setProperty("name", "quick/undo")
  .setProperty("title", "Undo Last Save")
  .setIcon(Icon::Edit::Undo).setText("Undo Last Save").onActivate([&] {
    program->loadState("quick/undo");
  }));
  loadState.append(MenuItem()
  .setProperty("name", "quick/redo")
  .setProperty("title", "Redo Last Undo")
  .setIcon(Icon::Edit::Redo).setText("Redo Last Undo").onActivate([&] {
    program->loadState("quick/redo");
  }));
  loadState.append(MenuItem().setIcon(Icon::Edit::Clear).setText("Remove All States").onActivate([&] {
    if(MessageDialog("Are you sure you want to permanently remove all quick states for this game?").setParent(*this).question() == "Yes") {
      for(uint index : range(QuickStates)) program->removeState({"quick/slot ", 1 + index});
      program->removeState("quick/undo");
      program->removeState("quick/redo");
      updateStateMenus();
    }
  }));
  speedMenu.setIcon(Icon::Device::Clock).setText("Speed");
  speedSlowest.setText("50% (Slowest)").setProperty("multiplier", "2.0").onActivate([&] { program->updateAudioFrequency(); });
  speedSlow.setText("75% (Slow)").setProperty("multiplier", "1.333").onActivate([&] { program->updateAudioFrequency(); });
  speedNormal.setText("100% (Normal)").setProperty("multiplier", "1.0").onActivate([&] { program->updateAudioFrequency(); });
  speedFast.setText("150% (Fast)").setProperty("multiplier", "0.667").onActivate([&] { program->updateAudioFrequency(); });
  speedFastest.setText("200% (Fastest)").setProperty("multiplier", "0.5").onActivate([&] { program->updateAudioFrequency(); });
  pauseEmulation.setText("Pause Emulation").onToggle([&] {
    if(pauseEmulation.checked()) audio.clear();
  });
  frameAdvance.setIcon(Icon::Media::Next).setText("Frame Advance").onActivate([&] {
    pauseEmulation.setChecked(false);
    program->frameAdvance = true;
  });
  captureScreenshot.setIcon(Icon::Emblem::Image).setText("Capture Screenshot").onActivate([&] {
    program->captureScreenshot();
  });
  cheatEditor.setIcon(Icon::Edit::Replace).setText("Cheat Editor ...").onActivate([&] { toolsWindow->show(0); });
  stateManager.setIcon(Icon::Application::FileManager).setText("State Manager ...").onActivate([&] { toolsWindow->show(1); });
  manifestViewer.setIcon(Icon::Emblem::Text).setText("Manifest Viewer ...").onActivate([&] { toolsWindow->show(2); });

  helpMenu.setText(tr("Help"));
  documentation.setIcon(Icon::Application::Browser).setText({tr("Documentation"), " ..."}).onActivate([&] {
    invoke("https://doc.byuu.org/bsnes/");
  });
  about.setIcon(Icon::Prompt::Question).setText({tr("About"), " ..."}).onActivate([&] {
    aboutWindow.setCentered(*this).setVisible().setFocused();
  });

  viewport.setDroppable().onDrop([&](vector<string> locations) {
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

  int ox = width - 144;
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
  if(!emulator->loaded()) viewportLayout.setPadding();
  if(!visible() || !video) return;

  uint32_t* output;
  uint length;
  uint width = 16;
  uint height = 16;
  if(video.acquire(output, length, width, height)) {
    for(uint y : range(height)) {
      auto line = output + y * (length >> 2);
      for(uint x : range(width)) *line++ = 0xff000000;
    }
    if(!emulator->loaded()) drawIcon(output, length, width, height);
    video.release();
    video.output();
  }
}

auto Presentation::resizeViewport() -> void {
  uint windowWidth = viewportLayout.geometry().width();
  uint windowHeight = viewportLayout.geometry().height();

  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 224.0 : 240.0);
  uint viewportWidth, viewportHeight;

  if(visible() && !fullScreen()) {
    uint widthMultiplier = windowWidth / width;
    uint heightMultiplier = windowHeight / height;
    uint multiplier = max(1, min(widthMultiplier, heightMultiplier));
    settings["View/Multiplier"].setValue(multiplier);
    for(auto item : sizeGroup.objects<MenuRadioItem>()) {
      if(auto property = item->property("multiplier")) {
        if(property.natural() == multiplier) item->setChecked();
      }
    }
  }

  if(!emulator->loaded()) return clearViewport();
  if(!video) return;

  if(settings["View/Output"].text() == "Center") {
    uint widthMultiplier = windowWidth / width;
    uint heightMultiplier = windowHeight / height;
    uint multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  } else if(settings["View/Output"].text() == "Scale") {
    double widthMultiplier = (double)windowWidth / width;
    double heightMultiplier = (double)windowHeight / height;
    double multiplier = min(widthMultiplier, heightMultiplier);
    viewportWidth = width * multiplier;
    viewportHeight = height * multiplier;
  } else if(settings["View/Output"].text() == "Stretch" || 1) {
    viewportWidth = windowWidth;
    viewportHeight = windowHeight;
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
  if(fullScreen()) return;
  if(maximized()) setMaximized(false);

  uint width = 256 * (settings["View/AspectCorrection"].boolean() ? 8.0 / 7.0 : 1.0);
  uint height = (settings["View/OverscanCropping"].boolean() ? 224.0 : 240.0);
  uint statusHeight = settings["UserInterface/ShowStatusBar"].boolean() ? StatusHeight : 0;

  uint multiplier = settings["View/Multiplier"].natural();
  if(!multiplier) multiplier = 2;

  setMinimumSize({width, height + StatusHeight});
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
    video.setExclusive(settings["Video/Exclusive"].boolean());
    if(video.exclusive()) setVisible(false);
    if(!input.acquired()) input.acquire();
    resizeViewport();
  } else {
    if(input.acquired()) input.release();
    if(video.exclusive()) setVisible(true);
    video.setExclusive(false);
    setFullScreen(false);
    menuBar.setVisible(true);
    if(settings["UserInterface/ShowStatusBar"].boolean()) {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    resizeWindow();
    setCentered();
  }
}

auto Presentation::updateDeviceMenu() -> void {
  controllerPort1.reset();
  controllerPort2.reset();
  expansionPort.reset();

  auto information = emulator->information();
  for(auto& port : emulator->ports()) {
    Menu* menu = nullptr;
    if(port.name == "Controller Port 1") menu = &controllerPort1;
    if(port.name == "Controller Port 2") menu = &controllerPort2;
    if(port.name == "Expansion Port") menu = &expansionPort;
    if(!menu) continue;

    auto path = string{information.name, "/", port.name}.replace(" ", "");
    auto deviceName = settings(path).text();
    auto deviceID = emulator->connected(port.id);

    Group devices;
    for(auto& device : emulator->devices(port.id)) {
      if(port.name == "Expansion Port" && device.name == "21fx") continue;

      MenuRadioItem item{menu};
      item.setProperty("deviceID", device.id);
      item.setText(tr(device.name));
      item.onActivate([=] {
        settings(path).setValue(device.name);
        emulator->connect(port.id, device.id);
        updateDeviceSelections();
      });
      devices.append(item);

      if(deviceName == device.name) item.doActivate();
      if(!deviceName && deviceID == device.id) item.doActivate();
    }
  }

  updateDeviceSelections();
}

auto Presentation::updateDeviceSelections() -> void {
  for(auto& port : emulator->ports()) {
    Menu* menu = nullptr;
    if(port.name == "Controller Port 1") menu = &controllerPort1;
    if(port.name == "Controller Port 2") menu = &controllerPort2;
    if(port.name == "Expansion Port") menu = &expansionPort;
    if(!menu) continue;

    auto deviceID = emulator->connected(port.id);
    for(auto& action : menu->actions()) {
      if(auto item = action.cast<MenuRadioItem>()) {
        if(item.property("deviceID").natural() == deviceID) {
          item.setChecked();
          break;
        }
      }
    }
  }
}

//generate a list of size multipliers
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

auto Presentation::updateStateMenus() -> void {
  auto states = program->availableStates("quick/");

  for(auto& action : saveState.actions()) {
    if(auto item = action.cast<MenuItem>()) {
      if(auto name = item.property("name")) {
        if(states.find(name)) {
          auto timestamp = program->stateTimestamp(item.property("name"));
          item.setText({item.property("title"), " [", chrono::local::datetime(timestamp), "]"});
        } else {
          item.setText({item.property("title"), " [Empty]"});
        }
      }
    }
  }

  for(auto& action : loadState.actions()) {
    if(auto item = action.cast<MenuItem>()) {
      if(auto name = item.property("name")) {
        if(states.find(name)) {
          auto timestamp = program->stateTimestamp(item.property("name"));
          item.setEnabled(true);
          item.setText({item.property("title"), " [", chrono::local::datetime(timestamp), "]"});
        } else {
          item.setEnabled(false);
          item.setText({item.property("title"), " [Empty]"});
        }
      }
    }
  }
}

auto Presentation::updateRecentGames() -> void {
  loadRecentGame.reset();

  //remove missing games from list
  for(uint index = 0; index < RecentGames;) {
    auto games = settings[string{"Game/Recent/", 1 + index}].text();
    bool missing = false;
    if(games) {
      for(auto& game : games.split("|")) {
        if(!inode::exists(game)) missing = true;
      }
    }
    if(missing) {
      //will read one past the end of Games/Recent[RecentGames] by design:
      //this will always return an empty string to clear the last item in the list
      for(uint offset = index; offset < RecentGames; offset++) {
        settings[string{"Game/Recent/", 1 + offset}].setValue(
          settings[string{"Game/Recent/", 2 + offset}].text()
        );
      }
    } else {
      index++;
    }
  }

  //update list
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
      item.setText({"[", tr("Empty"), "]"});
      item.setEnabled(false);
    }
    loadRecentGame.append(item);
  }

  loadRecentGame.append(MenuSeparator());
  loadRecentGame.append(MenuItem().setIcon(Icon::Edit::Clear).setText(tr("Clear List")).onActivate([&] {
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
      for(uint n : reverse(range(index + 1))) {
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
      item.setText(string{shader}.trimRight(".shader/", 1L)).onActivate([=] {
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
