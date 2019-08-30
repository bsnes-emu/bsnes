#include "../bsnes.hpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "paths.cpp"
#include "speed.cpp"
#include "emulator.cpp"
#include "drivers.cpp"
Settings settings;
VideoSettings videoSettings;
AudioSettings audioSettings;
InputSettings inputSettings;
HotkeySettings hotkeySettings;
PathSettings pathSettings;
SpeedSettings speedSettings;
EmulatorSettings emulatorSettings;
DriverSettings driverSettings;
namespace Instances { Instance<SettingsWindow> settingsWindow; }
SettingsWindow& settingsWindow = Instances::settingsWindow();

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
    video.driver = ruby::Video::optimalDriver();
    audio.driver = ruby::Audio::optimalDriver();
    input.driver = ruby::Input::optimalDriver();
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
  bind(text,    "Video/Monitor",   video.monitor);
  bind(text,    "Video/Format",    video.format);
  bind(text,    "Video/Shader",    video.shader);

  bind(natural, "Video/Luminance",  video.luminance);
  bind(natural, "Video/Saturation", video.saturation);
  bind(natural, "Video/Gamma",      video.gamma);
  bind(boolean, "Video/Dimming",    video.dimming);
  bind(boolean, "Video/Snow",       video.snow);

  bind(text,    "Video/Output",           video.output);
  bind(natural, "Video/Multiplier",       video.multiplier);
  bind(boolean, "Video/AspectCorrection", video.aspectCorrection);
  bind(boolean, "Video/Overscan",         video.overscan);
  bind(boolean, "Video/Blur",             video.blur);
  bind(text,    "Video/Filter",           video.filter);

  bind(text,    "Audio/Driver",    audio.driver);
  bind(boolean, "Audio/Exclusive", audio.exclusive);
  bind(text,    "Audio/Device",    audio.device);
  bind(boolean, "Audio/Blocking",  audio.blocking);
  bind(boolean, "Audio/Dynamic",   audio.dynamic);
  bind(natural, "Audio/Frequency", audio.frequency);
  bind(natural, "Audio/Latency",   audio.latency);

  bind(boolean, "Audio/Mute",          audio.mute);
  bind(boolean, "Audio/MuteUnfocused", audio.muteUnfocused);
  bind(integer, "Audio/Skew",          audio.skew);
  bind(natural, "Audio/Volume",        audio.volume);
  bind(natural, "Audio/Balance",       audio.balance);

  bind(text,    "Input/Driver",          input.driver);
  bind(natural, "Input/Frequency",       input.frequency);
  bind(text,    "Input/Defocus",         input.defocus);
  bind(natural, "Input/Turbo/Frequency", input.turbo.frequency);
  bind(text,    "Input/Hotkey/Logic",    input.hotkey.logic);

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

  bind(natural, "FastForward/FrameSkip", fastForward.frameSkip);
  bind(natural, "FastForward/Limiter",   fastForward.limiter);
  bind(boolean, "FastForward/Mute",      fastForward.mute);

  bind(natural, "Rewind/Frequency", rewind.frequency);
  bind(natural, "Rewind/Length",    rewind.length);
  bind(boolean, "Rewind/Mute",      rewind.mute);

  bind(boolean, "Emulator/WarnOnUnverifiedGames",        emulator.warnOnUnverifiedGames);
  bind(boolean, "Emulator/AutoSaveMemory/Enable",        emulator.autoSaveMemory.enable);
  bind(natural, "Emulator/AutoSaveMemory/Interval",      emulator.autoSaveMemory.interval);
  bind(boolean, "Emulator/AutoSaveStateOnUnload",        emulator.autoSaveStateOnUnload);
  bind(boolean, "Emulator/AutoLoadStateOnLoad",          emulator.autoLoadStateOnLoad);
  bind(text,    "Emulator/Hack/Entropy",                 emulator.hack.entropy);
  bind(natural, "Emulator/Hack/CPU/Overclock",           emulator.hack.cpu.overclock);
  bind(boolean, "Emulator/Hack/PPU/Fast",                emulator.hack.ppu.fast);
  bind(boolean, "Emulator/Hack/PPU/Deinterlace",         emulator.hack.ppu.deinterlace);
  bind(boolean, "Emulator/Hack/PPU/NoSpriteLimit",       emulator.hack.ppu.noSpriteLimit);
  bind(natural, "Emulator/Hack/PPU/Mode7/Scale",         emulator.hack.ppu.mode7.scale);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Perspective",   emulator.hack.ppu.mode7.perspective);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Supersample",   emulator.hack.ppu.mode7.supersample);
  bind(boolean, "Emulator/Hack/PPU/Mode7/Mosaic",        emulator.hack.ppu.mode7.mosaic);
  bind(boolean, "Emulator/Hack/DSP/Fast",                emulator.hack.dsp.fast);
  bind(boolean, "Emulator/Hack/DSP/Cubic",               emulator.hack.dsp.cubic);
  bind(boolean, "Emulator/Hack/Coprocessor/DelayedSync", emulator.hack.coprocessor.delayedSync);
  bind(boolean, "Emulator/Hack/Coprocessor/PreferHLE",   emulator.hack.coprocessor.preferHLE);
  bind(natural, "Emulator/Hack/SA1/Overclock",           emulator.hack.sa1.overclock);
  bind(natural, "Emulator/Hack/SuperFX/Overclock",       emulator.hack.superfx.overclock);
  bind(boolean, "Emulator/Cheats/Enable",                emulator.cheats.enable);

  bind(boolean, "General/StatusBar",   general.statusBar);
  bind(boolean, "General/ScreenSaver", general.screenSaver);
  bind(boolean, "General/ToolTips",    general.toolTips);
  bind(boolean, "General/Crashed",     general.crashed);
  bind(boolean, "General/BetaWarning", general.betaWarning);

  #undef bind
}

struct SettingsHome : VerticalLayout {
  SettingsHome() {
    setCollapsible();
    setVisible(false);
    image icon{Resource::Icon};
    auto data = icon.data();
    for(uint y : range(icon.height())) {
      for(uint x : range(icon.width())) {
        auto pixel = icon.read(data);
        auto a = pixel >> 24 & 255;
        auto r = pixel >> 16 & 255;
        auto g = pixel >>  8 & 255;
        auto b = pixel >>  0 & 255;
        a = a * 0.25;
        icon.write(data, a << 24 | r << 16 | g << 8 | b << 0);
        data += icon.stride();
      }
    }
    canvas.setIcon(icon);
  }

public:
  Canvas canvas{this, Size{~0, ~0}};
} settingsHome;

auto SettingsWindow::create() -> void {
  layout.setPadding(5_sx);
  panelList.append(ListViewItem().setText("Video").setIcon(Icon::Device::Display));
  panelList.append(ListViewItem().setText("Audio").setIcon(Icon::Device::Speaker));
  panelList.append(ListViewItem().setText("Input").setIcon(Icon::Device::Joypad));
  panelList.append(ListViewItem().setText("Hotkeys").setIcon(Icon::Device::Keyboard));
  panelList.append(ListViewItem().setText("Paths").setIcon(Icon::Emblem::Folder));
  panelList.append(ListViewItem().setText("Speed").setIcon(Icon::Device::Clock));
  panelList.append(ListViewItem().setText("Emulator").setIcon(Icon::Action::Settings));
  panelList.append(ListViewItem().setText("Drivers").setIcon(Icon::Place::Settings));
  panelList.onChange([&] {
    if(auto item = panelList.selected()) {
      show(item.offset());
    } else {
      show(-1);
    }
  });
  panelContainer.append(settingsHome, Size{~0, ~0});
  panelContainer.append(videoSettings, Size{~0, ~0});
  panelContainer.append(audioSettings, Size{~0, ~0});
  panelContainer.append(inputSettings, Size{~0, ~0});
  panelContainer.append(hotkeySettings, Size{~0, ~0});
  panelContainer.append(pathSettings, Size{~0, ~0});
  panelContainer.append(speedSettings, Size{~0, ~0});
  panelContainer.append(emulatorSettings, Size{~0, ~0});
  panelContainer.append(driverSettings, Size{~0, ~0});
  statusBar.setFont(Font().setBold());

  setTitle("Settings");
  setSize({680_sx, 400_sx});
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

auto SettingsWindow::show(int index) -> void {
  settingsHome.setVisible(false);
  videoSettings.setVisible(false);
  audioSettings.setVisible(false);
  inputSettings.setVisible(false);
  hotkeySettings.setVisible(false);
  pathSettings.setVisible(false);
  speedSettings.setVisible(false);
  emulatorSettings.setVisible(false);
  driverSettings.setVisible(false);
  panelList.item(index).setSelected();
  if(index ==-1) settingsHome.setVisible(true);
  if(index == 0) videoSettings.setVisible(true);
  if(index == 1) audioSettings.setVisible(true);
  if(index == 2) inputSettings.setVisible(true);
  if(index == 3) hotkeySettings.setVisible(true);
  if(index == 4) pathSettings.setVisible(true);
  if(index == 5) speedSettings.setVisible(true);
  if(index == 6) emulatorSettings.setVisible(true);
  if(index == 7) driverSettings.setVisible(true);
  panelContainer.resize();
  setVisible();
  setFocused();
}
