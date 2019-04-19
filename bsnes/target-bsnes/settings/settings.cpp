#include "../bsnes.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "paths.cpp"
#include "emulator.cpp"
#include "drivers.cpp"
Settings settings;
VideoSettings videoSettings;
AudioSettings audioSettings;
InputSettings inputSettings;
HotkeySettings hotkeySettings;
PathSettings pathSettings;
EmulatorSettings emulatorSettings;
DriverSettings driverSettings;
SettingsWindow settingsWindow;

auto Settings::load() -> void {
  Markup::Node::operator=(BML::unserialize(string::read(location), " "));
  process(true);
  file::write(locate("settings.bml"), BML::serialize(*this, " "));
}

auto Settings::save() -> void {
  process(false);
  file::write(location ? location : locate("settings.bml"), BML::serialize(*this, " "));
}

auto Settings::process(bool load) -> void {
  if(load) {
    //initialize non-static default settings
    video.driver = ruby::Video::safestDriver();
    audio.driver = ruby::Audio::safestDriver();
    input.driver = ruby::Input::safestDriver();
  }

  #define bind(type, path, name) \
    if(load) { \
      if(auto node = operator[](path)) name = node.type(); \
    } else { \
      operator()(path).setValue(name); \
    } \

  bind(text,    "Video/Driver",    video.driver);
  bind(boolean, "Video/Exclusive", video.exclusive);
  bind(boolean, "Video/Blocking",  video.blocking);
  bind(boolean, "Video/Flush",     video.flush);
  bind(text,    "Video/Format",    video.format);
  bind(text,    "Video/Shader",    video.shader);

  bind(natural, "Video/Luminance",  video.luminance);
  bind(natural, "Video/Saturation", video.saturation);
  bind(natural, "Video/Gamma",      video.gamma);

  bind(text,    "Video/Output",           video.output);
  bind(natural, "Video/Multiplier",       video.multiplier);
  bind(boolean, "Video/AspectCorrection", video.aspectCorrection);
  bind(boolean, "Video/Overscan",         video.overscan);
  bind(boolean, "Video/Blur",             video.blur);

  bind(text,    "Audio/Driver",    audio.driver);
  bind(boolean, "Audio/Exclusive", audio.exclusive);
  bind(text,    "Audio/Device",    audio.device);
  bind(boolean, "Audio/Blocking",  audio.blocking);
  bind(boolean, "Audio/Dynamic",   audio.dynamic);
  bind(natural, "Audio/Frequency", audio.frequency);
  bind(natural, "Audio/Latency",   audio.latency);

  bind(boolean, "Audio/Mute",    audio.mute);
  bind(integer, "Audio/Skew",    audio.skew);
  bind(natural, "Audio/Volume",  audio.volume);
  bind(natural, "Audio/Balance", audio.balance);

  bind(text,    "Input/Driver",          input.driver);
  bind(natural, "Input/Frequency",       input.frequency);
  bind(text,    "Input/Defocus",         input.defocus);
  bind(natural, "Input/Turbo/Frequency", input.turbo.frequency);

  bind(text,    "Path/Games",       path.games);
  bind(text,    "Path/Patches",     path.patches);
  bind(text,    "Path/Saves",       path.saves);
  bind(text,    "Path/Cheats",      path.cheats);
  bind(text,    "Path/States",      path.states);
  bind(text,    "Path/Screenshots", path.screenshots);

  bind(text,    "Path/Recent/SuperFamicom", path.recent.superFamicom);
  bind(text,    "Path/Recent/GameBoy",      path.recent.gameBoy);
  bind(text,    "Path/Recent/BSMemory",     path.recent.bsMemory);
  bind(text,    "Path/Recent/SufamiTurboA", path.recent.sufamiTurboA);
  bind(text,    "Path/Recent/SufamiTurboB", path.recent.sufamiTurboB);

  bind(boolean, "Emulator/WarnOnUnverifiedGames",         emulator.warnOnUnverifiedGames);
  bind(boolean, "Emulator/AutoSaveMemory/Enable",         emulator.autoSaveMemory.enable);
  bind(natural, "Emulator/AutoSaveMemory/Interval",       emulator.autoSaveMemory.interval);
  bind(boolean, "Emulator/AutoSaveStateOnUnload",         emulator.autoSaveStateOnUnload);
  bind(boolean, "Emulator/AutoLoadStateOnLoad",           emulator.autoLoadStateOnLoad);
  bind(boolean, "Emulator/Hack/PPU/Fast",                 emulator.hack.ppu.fast);
  bind(boolean, "Emulator/Hack/PPU/NoSpriteLimit",        emulator.hack.ppu.noSpriteLimit);
  bind(natural, "Emulator/Hack/PPU/Mode7/Scale",          emulator.hack.ppu.mode7.scale);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Perspective",    emulator.hack.ppu.mode7.perspective);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Supersample",    emulator.hack.ppu.mode7.supersample);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Mosaic",         emulator.hack.ppu.mode7.mosaic);
  bind(boolean, "Emulator/Hack/DSP/Fast",                 emulator.hack.dsp.fast);
  bind(boolean, "Emulator/Hack/DSP/Cubic",                emulator.hack.dsp.cubic);
  bind(boolean, "Emulator/Hack/Coprocessors/DelayedSync", emulator.hack.coprocessors.delayedSync);
  bind(boolean, "Emulator/Hack/Coprocessors/HLE",         emulator.hack.coprocessors.hle);
  bind(natural, "Emulator/Hack/FastSuperFX",              emulator.hack.fastSuperFX);
  bind(boolean, "Emulator/Cheats/Enable",                 emulator.cheats.enable);

  bind(boolean, "General/StatusBar",   general.statusBar);
  bind(boolean, "General/ScreenSaver", general.screenSaver);
  bind(boolean, "General/ToolTips",    general.toolTips);
  bind(boolean, "General/Crashed",     general.crashed);

  #undef bind
}

auto SettingsWindow::create() -> void {
  layout.setPadding(5_sx);
  panel.append(videoSettings);
  panel.append(audioSettings);
  panel.append(inputSettings);
  panel.append(hotkeySettings);
  panel.append(pathSettings);
  panel.append(emulatorSettings);
  panel.append(driverSettings);
  statusBar.setFont(Font().setBold());

  setTitle("Settings");
  setSize({600_sx, 400_sx});
  setAlignment({0.0, 1.0});
  setDismissable();

  onClose([&] {
    if(inputSettings.activeMapping) inputSettings.cancelMapping();
    if(hotkeySettings.activeMapping) hotkeySettings.cancelMapping();
    setVisible(false);
  });
}

auto SettingsWindow::setVisible(bool visible) -> SettingsWindow& {
  if(visible) {
    inputSettings.refreshMappings();
    hotkeySettings.refreshMappings();
    Application::processEvents();
  }
  return Window::setVisible(visible), *this;
}

auto SettingsWindow::show(uint index) -> void {
  panel.item(index).setSelected();
  setVisible();
  setFocused();
}
