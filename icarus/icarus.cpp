#include <nall/nall.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::userData(), "bsnes/"};
  if(inode::exists(location)) return location;

  directory::create({Path::userSettings(), "bsnes/"});
  return {Path::userSettings(), "bsnes/", name};
}

#include "settings.cpp"
Settings settings;

#include "heuristics/heuristics.hpp"
#include "heuristics/heuristics.cpp"
#include "heuristics/famicom.cpp"
#include "heuristics/super-famicom.cpp"
#include "heuristics/sg-1000.cpp"
#include "heuristics/sc-3000.cpp"
#include "heuristics/master-system.cpp"
#include "heuristics/mega-drive.cpp"
#include "heuristics/pc-engine.cpp"
#include "heuristics/supergrafx.cpp"
#include "heuristics/colecovision.cpp"
#include "heuristics/msx.cpp"
#include "heuristics/game-boy.cpp"
#include "heuristics/game-boy-advance.cpp"
#include "heuristics/game-gear.cpp"
#include "heuristics/wonderswan.cpp"
#include "heuristics/neo-geo-pocket.cpp"
#include "heuristics/neo-geo-pocket-color.cpp"
#include "heuristics/bs-memory.cpp"
#include "heuristics/sufami-turbo.cpp"

#include "core/core.hpp"
#include "core/core.cpp"
#include "core/famicom.cpp"
#include "core/super-famicom.cpp"
#include "core/sg-1000.cpp"
#include "core/sc-3000.cpp"
#include "core/master-system.cpp"
#include "core/mega-drive.cpp"
#include "core/pc-engine.cpp"
#include "core/supergrafx.cpp"
#include "core/colecovision.cpp"
#include "core/msx.cpp"
#include "core/game-boy.cpp"
#include "core/game-boy-color.cpp"
#include "core/game-boy-advance.cpp"
#include "core/game-gear.cpp"
#include "core/wonderswan.cpp"
#include "core/wonderswan-color.cpp"
#include "core/neo-geo-pocket.cpp"
#include "core/neo-geo-pocket-color.cpp"
#include "core/pocket-challenge-v2.cpp"
#include "core/bs-memory.cpp"
#include "core/sufami-turbo.cpp"

#if !defined(ICARUS_LIBRARY)

Icarus icarus;
#include "ui/ui.hpp"
#include "ui/scan-dialog.cpp"
#include "ui/settings-dialog.cpp"
#include "ui/import-dialog.cpp"
#include "ui/error-dialog.cpp"

auto hiro::initialize() -> void {
  Application::setName("icarus");
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  if(arguments.size() == 1 && arguments[0] == "--name") {
    return print("icarus");
  }

  if(arguments.size() == 2 && arguments[0] == "--manifest" && directory::exists(arguments[1])) {
    return print(icarus.manifest(arguments[1]));
  }

  if(arguments.size() == 2 && arguments[0] == "--import" && file::exists(arguments[1])) {
    if(string target = icarus.import(arguments[1])) {
      return print(target, "\n");
    }
    return;
  }

  if(arguments.size() == 1 && arguments[0] == "--import") {
    if(string source = BrowserDialog()
    .setTitle("Load ROM File")
    .setPath(settings["icarus/Path"].text())
    .setFilters("ROM Files|"
      "*.fc:*.nes:"
      "*.sfc:*.smc:"
      "*.sg1000:*.sg:"
      "*.sc3000:*.sc:"
      "*.ms:*.sms:"
      "*.md:*.smd:*.gen:"
      "*.pce:"
      "*.sgx:"
      "*.cv:*.col:"
      "*.msx:"
      "*.gb:"
      "*.gbc:"
      "*.gba:"
      "*.gg:"
      "*.ws:"
      "*.wsc:"
      "*.pc2:"
      "*.ngp:"
      "*.ngpc:*.ngc:"
      "*.bs:"
      "*.st:"
      "*.zip"
    ).openFile()) {
      if(string target = icarus.import(source)) {
        settings["icarus/Path"].setValue(Location::path(source));
        return print(target, "\n");
      }
    }
    return;
  }

  new ScanDialog;
  new SettingsDialog;
  new ImportDialog;
  new ErrorDialog;

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] {
    MessageDialog().setTitle("About icarus").setText({
      "icarus\n\n"
      "Author: byuu\n"
      "License: GPLv3\n"
      "Website: https://byuu.org/\n"
    }).information();
  });
  Application::Cocoa::onPreferences([&] {
    scanDialog->settingsButton.doActivate();
  });
  Application::Cocoa::onQuit([&] {
    Application::quit();
  });
  #endif

  scanDialog->show();
  Application::run();
  settings.save();
}

#endif
