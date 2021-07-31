#include "../bsnes.hpp"
namespace Instances { Instance<Presentation> presentation; }
Presentation& presentation = Instances::presentation();

auto Presentation::create() -> void {
  systemMenu.setText(tr("System"));
  loadGame.setIcon(Icon::Action::Open).setText({tr("Load Game"), " ..."}).onActivate([&] {
    program.load();
  });
  loadRecentGame.setIcon(Icon::Action::Open).setText(tr("Load Recent Game"));
  updateRecentGames();
  resetSystem.setIcon(Icon::Action::Refresh).setText(tr("Reset System")).setEnabled(false).onActivate([&] {
    program.reset();
  });
  unloadGame.setIcon(Icon::Action::Remove).setText(tr("Unload Game")).setEnabled(false).onActivate([&] {
    program.unload();
  });
  controllerPort1.setIcon(Icon::Device::Joypad).setText(tr("Controller Port 1"));
  controllerPort2.setIcon(Icon::Device::Joypad).setText(tr("Controller Port 2"));
  expansionPort.setIcon(Icon::Device::Storage).setText(tr("Expansion Port"));
  updateDeviceMenu();
  quit.setIcon(Icon::Action::Quit).setText(tr("Quit")).onActivate([&] { program.quit(); });

  settingsMenu.setText(tr("Settings"));
  sizeMenu.setIcon(Icon::Emblem::Image).setText("Size");
  updateSizeMenu();
  outputMenu.setIcon(Icon::Emblem::Image).setText("Output");
  centerViewport.setText("Center").onActivate([&] {
    settings.video.output = "Center";
    video.clear();
  });
  scaleViewport.setText("Scale").onActivate([&] {
    settings.video.output = "Scale";
    video.clear();
  });
  stretchViewport.setText("Stretch").onActivate([&] {
    settings.video.output = "Stretch";
    video.clear();
  });
  if(settings.video.output == "Center") centerViewport.setChecked();
  if(settings.video.output == "Scale") scaleViewport.setChecked();
  if(settings.video.output == "Stretch") stretchViewport.setChecked();
  aspectCorrection.setText("Aspect Correction").setChecked(settings.video.aspectCorrection).onToggle([&] {
    settings.video.aspectCorrection = aspectCorrection.checked();
    resizeWindow();
  });
  showOverscanArea.setText("Show Overscan Area").setChecked(settings.video.overscan).onToggle([&] {
    settings.video.overscan = showOverscanArea.checked();
    resizeWindow();
  });
  blurEmulation.setText("Hires Blur Emulation").setChecked(settings.video.blur).onToggle([&] {
    settings.video.blur = blurEmulation.checked();
    emulator->configure("Video/BlurEmulation", settings.video.blur);
  }).doToggle();
  filterMenu.setIcon(Icon::Emblem::Image).setText("Filter");
  filterNone.setText("None").onActivate([&] { settings.video.filter = "None"; });
  filterScanlinesLight.setText("Scanlines (66%)").onActivate([&] { settings.video.filter = "Scanlines (66%)"; });
  filterScanlinesDark.setText("Scanlines (33%)").onActivate([&] { settings.video.filter = "Scanlines (33%)"; });
  filterScanlinesBlack.setText("Scanlines (0%)").onActivate([&] { settings.video.filter = "Scanlines (0%)"; });
  filterPixellate2x.setText("Pixellate2x").onActivate([&] { settings.video.filter = "Pixellate2x"; });
  filterScale2x.setText("Scale2x").onActivate([&] { settings.video.filter = "Scale2x"; });
  filter2xSaI.setText("2xSaI").onActivate([&] { settings.video.filter = "2xSaI"; });
  filterSuper2xSaI.setText("Super 2xSaI").onActivate([&] { settings.video.filter = "Super 2xSaI"; });
  filterSuperEagle.setText("Super Eagle").onActivate([&] { settings.video.filter = "Super Eagle"; });
  filterLQ2x.setText("LQ2x").onActivate([&] { settings.video.filter = "LQ2x"; });
  filterHQ2x.setText("HQ2x").onActivate([&] { settings.video.filter = "HQ2x"; });
  filterNTSC_RF.setText("NTSC (RF)").onActivate([&] { settings.video.filter = "NTSC (RF)"; });
  filterNTSC_Composite.setText("NTSC (Composite)").onActivate([&] { settings.video.filter = "NTSC (Composite)"; });
  filterNTSC_SVideo.setText("NTSC (S-Video)").onActivate([&] { settings.video.filter = "NTSC (S-Video)"; });
  filterNTSC_RGB.setText("NTSC (RGB)").onActivate([&] { settings.video.filter = "NTSC (RGB)"; });
  if(settings.video.filter == "None") filterNone.setChecked();
  if(settings.video.filter == "Scanlines (66%)") filterScanlinesLight.setChecked();
  if(settings.video.filter == "Scanlines (33%)") filterScanlinesDark.setChecked();
  if(settings.video.filter == "Scanlines (0%)") filterScanlinesBlack.setChecked();
  if(settings.video.filter == "Pixellate2x") filterPixellate2x.setChecked();
  if(settings.video.filter == "Scale2x") filterScale2x.setChecked();
  if(settings.video.filter == "2xSaI") filter2xSaI.setChecked();
  if(settings.video.filter == "Super 2xSaI") filterSuper2xSaI.setChecked();
  if(settings.video.filter == "Super Eagle") filterSuperEagle.setChecked();
  if(settings.video.filter == "LQ2x") filterLQ2x.setChecked();
  if(settings.video.filter == "HQ2x") filterHQ2x.setChecked();
  if(settings.video.filter == "NTSC (RF)") filterNTSC_RF.setChecked();
  if(settings.video.filter == "NTSC (Composite)") filterNTSC_Composite.setChecked();
  if(settings.video.filter == "NTSC (S-Video)") filterNTSC_SVideo.setChecked();
  if(settings.video.filter == "NTSC (RGB)") filterNTSC_RGB.setChecked();
  shaderMenu.setIcon(Icon::Emblem::Image).setText("Shader");
  muteAudio.setText("Mute Audio").setChecked(settings.audio.mute).onToggle([&] {
    settings.audio.mute = muteAudio.checked();
    if(settings.audio.mute) {
      program.mute |= Program::Mute::Always;
    } else {
      program.mute &= ~Program::Mute::Always;
    }
  }).doToggle();  //set initial mute state flag
  showStatusBar.setText("Show Status Bar").setChecked(settings.general.statusBar).onToggle([&] {
    settings.general.statusBar = showStatusBar.checked();
    if(!showStatusBar.checked()) {
      layout.remove(statusLayout);
    } else {
      layout.append(statusLayout, Size{~0, StatusHeight});
    }
    if(visible()) resizeWindow();
  });
  videoSettings.setIcon(Icon::Device::Display).setText("Video ...").onActivate([&] { settingsWindow.show(0); });
  audioSettings.setIcon(Icon::Device::Speaker).setText("Audio ...").onActivate([&] { settingsWindow.show(1); });
  inputSettings.setIcon(Icon::Device::Joypad).setText("Input ...").onActivate([&] { settingsWindow.show(2); });
  hotkeySettings.setIcon(Icon::Device::Keyboard).setText("Hotkeys ...").onActivate([&] { settingsWindow.show(3); });
  pathSettings.setIcon(Icon::Emblem::Folder).setText("Paths ...").onActivate([&] { settingsWindow.show(4); });
  emulatorSettings.setIcon(Icon::Action::Settings).setText("Emulator ...").onActivate([&] { settingsWindow.show(5); });
  enhancementSettings.setIcon(Icon::Action::Add).setText("Enhancements ...").onActivate([&] { settingsWindow.show(6); });
  compatibilitySettings.setIcon(Icon::Action::Remove).setText("Compatibility ...").onActivate([&] { settingsWindow.show(7); });
  driverSettings.setIcon(Icon::Place::Settings).setText("Drivers ...").onActivate([&] { settingsWindow.show(8); });

  toolsMenu.setText(tr("Tools")).setVisible(false);
  saveState.setIcon(Icon::Media::Record).setText("Save State");
  for(uint index : range(QuickStates)) {
    MenuItem item{&saveState};
    item.setAttribute("name", {"Quick/Slot ", 1 + index});
    item.setAttribute("title", {"Slot ", 1 + index});
    item.setText({"Slot ", 1 + index});
    item.onActivate([=] { program.saveState({"Quick/Slot ", 1 + index}); });
  }
  loadState.setIcon(Icon::Media::Rewind).setText("Load State");
  for(uint index : range(QuickStates)) {
    MenuItem item{&loadState};
    item.setAttribute("name", {"Quick/Slot ", 1 + index});
    item.setAttribute("title", {"Slot ", 1 + index});
    item.setText({"Slot ", 1 + index});
    item.onActivate([=] { program.loadState({"Quick/Slot ", 1 + index}); });
  }
  loadState.append(MenuSeparator());
  loadState.append(MenuItem()
  .setAttribute("name", "Quick/Undo")
  .setAttribute("title", "Undo Last Save")
  .setIcon(Icon::Edit::Undo).setText("Undo Last Save").onActivate([&] {
    program.loadState("Quick/Undo");
  }));
  loadState.append(MenuItem()
  .setAttribute("name", "Quick/Redo")
  .setAttribute("title", "Redo Last Undo")
  .setIcon(Icon::Edit::Redo).setText("Redo Last Undo").onActivate([&] {
    program.loadState("Quick/Redo");
  }));
  loadState.append(MenuItem().setIcon(Icon::Edit::Clear).setText("Remove All States").onActivate([&] {
    if(MessageDialog("Are you sure you want to permanently remove all quick states for this game?").setAlignment(*this).question() == "Yes") {
      for(uint index : range(QuickStates)) program.removeState({"Quick/Slot ", 1 + index});
      program.removeState("Quick/Undo");
      program.removeState("Quick/Redo");
    }
  }));
  speedMenu.setIcon(Icon::Device::Clock).setText("Speed").setEnabled(!settings.video.blocking && settings.audio.blocking);
  speedSlowest.setText("50% (Slowest)").setAttribute("multiplier", "2.0").onActivate([&] { program.updateAudioFrequency(); });
  speedSlow.setText("75% (Slow)").setAttribute("multiplier", "1.333").onActivate([&] { program.updateAudioFrequency(); });
  speedNormal.setText("100% (Normal)").setAttribute("multiplier", "1.0").onActivate([&] { program.updateAudioFrequency(); });
  speedFast.setText("150% (Fast)").setAttribute("multiplier", "0.667").onActivate([&] { program.updateAudioFrequency(); });
  speedFastest.setText("200% (Fastest)").setAttribute("multiplier", "0.5").onActivate([&] { program.updateAudioFrequency(); });
  runMenu.setIcon(Icon::Media::Play).setText("Run Mode");
  runEmulation.setText("Normal").onActivate([&] {
  });
  pauseEmulation.setText("Pause Emulation").onActivate([&] {
    audio.clear();
  });
  frameAdvance.setText("Frame Advance").onActivate([&] {
    audio.clear();
    program.frameAdvanceLock = true;
  });
  movieMenu.setIcon(Icon::Emblem::Video).setText("Movie");
  moviePlay.setIcon(Icon::Media::Play).setText("Play").onActivate([&] { program.moviePlay(); });
  movieRecord.setIcon(Icon::Media::Record).setText("Record").onActivate([&] { program.movieRecord(false); });
  movieRecordFromBeginning.setIcon(Icon::Media::Record).setText("Reset and Record").onActivate([&] { program.movieRecord(true); });
  movieStop.setIcon(Icon::Media::Stop).setText("Stop").onActivate([&] { program.movieStop(); });
  captureScreenshot.setIcon(Icon::Emblem::Image).setText("Capture Screenshot").onActivate([&] {
    program.captureScreenshot();
  });
  cheatFinder.setIcon(Icon::Action::Search).setText("Cheat Finder ...").onActivate([&] { toolsWindow.show(0); });
  cheatEditor.setIcon(Icon::Edit::Replace).setText("Cheat Editor ...").onActivate([&] { toolsWindow.show(1); });
  stateManager.setIcon(Icon::Application::FileManager).setText("State Manager ...").onActivate([&] { toolsWindow.show(2); });
  manifestViewer.setIcon(Icon::Emblem::Text).setText("Manifest Viewer ...").onActivate([&] { toolsWindow.show(3); });

  helpMenu.setText(tr("Help"));
  aboutSameBoy.setIcon(Icon::Prompt::Question).setText({tr("About SameBoy"), " ..."}).onActivate([&] {
    AboutDialog()
    .setName("SameBoy")
    .setLogo(Resource::SameBoy)
    .setDescription("Super Game Boy emulator")
    .setVersion("0.14.4")
    .setCopyright("Lior Halphon")
    .setLicense("MIT")
    .setWebsite("https://sameboy.github.io")
    .setAlignment(*this)
    .show();
  });
  aboutBsnes.setIcon(Icon::Prompt::Question).setText({tr("About bsnes"), " ..."}).onActivate([&] {
    AboutDialog()
    .setName(Emulator::Name)
    .setLogo(Resource::Logo)
    .setDescription("Super Nintendo emulator")
    .setVersion(Emulator::Version)
    .setCopyright(Emulator::Copyright)
    .setLicense(Emulator::License)
    .setWebsite(Emulator::Website)
    .setAlignment(*this)
    .show();
  });

  viewport.setFocusable(false);  //true would also capture Alt, which breaks keyboard menu navigation
  viewport.setDroppable();
  viewport.onDrop([&](auto locations) { onDrop(locations); });

  iconSpacer.setColor({0, 0, 0});
  iconSpacer.setDroppable();
  iconSpacer.onDrop([&](auto locations) { onDrop(locations); });

  iconLayout.setAlignment(0.0).setCollapsible();
  image icon{Resource::Icon};
  icon.alphaBlend(0x000000);
  iconCanvas.setIcon(icon);
  iconCanvas.setDroppable();
  iconCanvas.onDrop([&](auto locations) { onDrop(locations); });

  iconPadding.setColor({0, 0, 0});
  iconPadding.setDroppable();
  iconPadding.onDrop([&](auto locations) { onDrop(locations); });

  if(!settings.general.statusBar) layout.remove(statusLayout);

  auto font = Font().setBold();
  auto back = Color{ 32,  32,  32};
  auto fore = Color{255, 255, 255};

  updateProgramIcon();
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

  program.updateStatus();

  onClose([&] {
    program.quit();
  });

  setTitle({"bsnes v", Emulator::Version});
  resizeWindow();
  setAlignment(Alignment::Center);

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] { aboutBsnes.doActivate(); });
  Application::Cocoa::onActivate([&] { setFocused(); });
  Application::Cocoa::onPreferences([&] { settingsWindow.show(2); });
  Application::Cocoa::onQuit([&] { doClose(); });
  #endif
}

auto Presentation::onDrop(vector<string> locations) -> void {
  if(!locations) return;
  program.gameQueue = {};
  program.gameQueue.append({"Auto;", locations.first()});
  program.load();
  setFocused();
}

auto Presentation::updateProgramIcon() -> void {
  presentation.iconLayout.setVisible(!emulator->loaded() && !settings.video.snow);
  presentation.layout.resize();
  //todo: video.clear() is not working on macOS/OpenGL 3.2
  if(auto [output, length] = video.acquire(1, 1); output) {
    *output = 0;
    video.release();
    video.output();
  }
}

auto Presentation::updateStatusIcon() -> void {
  image icon;
  icon.allocate(16, StatusHeight);
  icon.fill(0xff202020);

  if(emulator->loaded() && program.verified()) {
    image emblem{Icon::Emblem::Program};
    icon.impose(image::blend::sourceAlpha, 0, (StatusHeight - 16) / 2, emblem, 0, 0, 16, 16);
    statusIcon.setIcon(icon).setToolTip(
      "This is a known clean game image.\n"
      "PCB emulation is 100% accurate."
    );
  } else if(emulator->loaded()) {
    image emblem{Icon::Emblem::Binary};
    icon.impose(image::blend::sourceAlpha, 0, (StatusHeight - 16) / 2, emblem, 0, 0, 16, 16);
    statusIcon.setIcon(icon).setToolTip(
      "This is not a verified game image.\n"
      "PCB emulation is relying on heuristics."
    );
  } else {
    statusIcon.setIcon(icon).setToolTip();
  }
}

auto Presentation::resizeWindow() -> void {
  if(fullScreen()) return;
  if(maximized()) setMaximized(false);

  uint width = 256 * (settings.video.aspectCorrection ? 8.0 / 7.0 : 1.0);
  uint height = (settings.video.overscan ? 240.0 : 224.0);
  uint multiplier = max(1, settings.video.multiplier);
  uint statusHeight = settings.general.statusBar ? StatusHeight : 0;

  setMinimumSize({width, height + statusHeight});
  setSize({width * multiplier, height * multiplier + statusHeight});
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
      item.setAttribute("deviceID", device.id);
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
        if(item.attribute("deviceID").natural() == deviceID) {
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
    item.setAttribute("multiplier", multiplier);
    item.setText({multiplier, "x (", 240 * multiplier, "p)"});
    item.onActivate([=] {
      settings.video.multiplier = multiplier;
      resizeWindow();
    });
    sizeGroup.append(item);
  }
  for(auto item : sizeGroup.objects<MenuRadioItem>()) {
    if(settings.video.multiplier == item.attribute("multiplier").natural()) {
      item.setChecked();
    }
  }

  sizeMenu.append(MenuSeparator());
  sizeMenu.append(MenuItem().setIcon(Icon::Action::Remove).setText("Shrink Window To Size").onActivate([&] {
    resizeWindow();
  }));
  sizeMenu.append(MenuItem().setIcon(Icon::Place::Settings).setText("Center Window").onActivate([&] {
    setAlignment(Alignment::Center);
  }));
}

auto Presentation::updateStateMenus() -> void {
  auto states = program.availableStates("Quick/");

  for(auto& action : saveState.actions()) {
    if(auto item = action.cast<MenuItem>()) {
      if(auto name = item.attribute("name")) {
        if(auto offset = states.find([&](auto& state) { return state.name == name; })) {
          item.setText({item.attribute("title"), " (", chrono::local::datetime(states[*offset].date), ")"});
        } else {
          item.setText({item.attribute("title"), " (empty)"});
        }
      }
    }
  }

  for(auto& action : loadState.actions()) {
    if(auto item = action.cast<MenuItem>()) {
      if(auto name = item.attribute("name")) {
        if(auto offset = states.find([&](auto& state) { return state.name == name; })) {
          item.setEnabled(true);
          item.setText({item.attribute("title"), " (", chrono::local::datetime(states[*offset].date), ")"});
        } else {
          item.setEnabled(false);
          item.setText({item.attribute("title"), " (empty)"});
        }
      }
    }
  }
}

auto Presentation::updateRecentGames() -> void {
  loadRecentGame.reset();

  //remove missing games from list
  for(uint index = 0; index < RecentGames;) {
    auto games = settings[{"Game/Recent/", 1 + index}].text();
    bool missing = false;
    if(games) {
      for(auto& game : games.split("|")) {
        if(!inode::exists(game.split(";").last())) missing = true;
      }
    }
    if(missing) {
      //will read one past the end of Games/Recent[RecentGames] by design:
      //this will always return an empty string to clear the last item in the list
      for(uint offset = index; offset < RecentGames; offset++) {
        settings[{"Game/Recent/", 1 + offset}].setValue(
          settings[{"Game/Recent/", 2 + offset}].text()
        );
      }
    } else {
      index++;
    }
  }

  //replace any empty option strings with "Auto", to work around a bug in bsnes
  //v115 and below.
  for(auto entry : settings[{"Game/Recent"}]) {
    auto old_games = entry.text();
    if(!old_games) continue;
    vector<string> new_games;
    for(auto& game : old_games.split("|")) {
        auto parts = game.split(";", 1L);
        if(parts[0] == "") parts[0] = "Auto";
        new_games.append(parts.merge(";"));
    }
    entry.setValue(new_games.merge("|"));
  }

  //update list
  for(auto index : range(RecentGames)) {
    MenuItem item{&loadRecentGame};
    if(auto game = settings[{"Game/Recent/", 1 + index}].text()) {
      string displayName;
      auto games = game.split("|");
      for(auto& game : games) {
        displayName.append(Location::prefix(game.split(";", 1L).last()), " + ");
      }
      displayName.trimRight(" + ", 1L);
      item.setIcon(games(0).endsWith("/") ? (image)Icon::Action::Open : (image)Icon::Emblem::File);
      item.setText(displayName);
      item.onActivate([=] {
        program.gameQueue = games;
        program.load();
      });
    } else {
      item.setText({"(", tr("empty"), ")"});
      item.setEnabled(false);
    }
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
  shaderMenu.setEnabled(video.hasShader());
  if(!video.hasShader()) return;

  Group shaders;

  MenuRadioItem none{&shaderMenu};
  none.setText("None").onActivate([&] {
    settings.video.shader = "None";
    program.updateVideoShader();
  });
  shaders.append(none);

  MenuRadioItem blur{&shaderMenu};
  blur.setText("Blur").onActivate([&] {
    settings.video.shader = "Blur";
    program.updateVideoShader();
  });
  shaders.append(blur);

  auto location = locate("Shaders/");

  if(settings.video.driver == "OpenGL 3.2") {
    for(auto shader : directory::folders(location, "*.shader")) {
      if(shaders.objectCount() == 2) shaderMenu.append(MenuSeparator());
      MenuRadioItem item{&shaderMenu};
      item.setText(string{shader}.trimRight(".shader/", 1L)).onActivate([=] {
        settings.video.shader = {location, shader};
        program.updateVideoShader();
      });
      shaders.append(item);
    }
  }

  if(settings.video.shader == "None") none.setChecked();
  if(settings.video.shader == "Blur") blur.setChecked();
  for(auto item : shaders.objects<MenuRadioItem>()) {
    if(settings.video.shader == string{location, item.text(), ".shader/"}) {
      item.setChecked();
    }
  }
}
