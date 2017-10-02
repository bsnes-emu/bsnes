#include <nall/nall.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  location = {Path::config(), "icarus/", name};
  if(inode::exists(location)) return location;

  directory::create({Path::local(), "icarus/"});
  return {Path::local(), "icarus/", name};
}

#include "settings.cpp"
Settings settings;

#include "heuristics/famicom.cpp"
#include "heuristics/super-famicom.cpp"
#include "heuristics/master-system.cpp"
#include "heuristics/mega-drive.cpp"
#include "heuristics/pc-engine.cpp"
#include "heuristics/supergrafx.cpp"
#include "heuristics/game-boy.cpp"
#include "heuristics/game-boy-advance.cpp"
#include "heuristics/game-gear.cpp"
#include "heuristics/wonderswan.cpp"
#include "heuristics/bs-memory.cpp"
#include "heuristics/sufami-turbo.cpp"

#include "core/core.hpp"
#include "core/core.cpp"
#include "core/famicom.cpp"
#include "core/super-famicom.cpp"
#include "core/master-system.cpp"
#include "core/mega-drive.cpp"
#include "core/pc-engine.cpp"
#include "core/supergrafx.cpp"
#include "core/game-boy.cpp"
#include "core/game-boy-color.cpp"
#include "core/game-boy-advance.cpp"
#include "core/game-gear.cpp"
#include "core/wonderswan.cpp"
#include "core/wonderswan-color.cpp"
#include "core/bs-memory.cpp"
#include "core/sufami-turbo.cpp"

#if !defined(ICARUS_LIBRARY)

Icarus icarus;
#include "ui/ui.hpp"
#include "ui/scan-dialog.cpp"
#include "ui/settings-dialog.cpp"
#include "ui/import-dialog.cpp"
#include "ui/error-dialog.cpp"

#include <nall/main.hpp>
auto nall::main(string_vector args) -> void {
  Application::setName("icarus");

  if(args.size() == 2 && args[1] == "--name") {
    return print("icarus");
  }

  if(args.size() == 3 && args[1] == "--manifest" && directory::exists(args[2])) {
    return print(icarus.manifest(args[2]));
  }

  if(args.size() == 3 && args[1] == "--import" && file::exists(args[2])) {
    if(string target = icarus.import(args[2])) {
      return print(target, "\n");
    }
    return;
  }

  if(args.size() == 2 && args[1] == "--import") {
    if(string source = BrowserDialog()
    .setTitle("Load ROM Image")
    .setPath(settings["icarus/Path"].text())
    .setFilters("ROM Files|"
      "*.fc:*.nes:"
      "*.sfc:*.smc:"
      "*.ms:*.sms:"
      "*.md:*.smd:*.gen:"
      "*.pce:"
      "*.sg:*.sgx:"
      "*.gb:"
      "*.gbc:"
      "*.gba:"
      "*.gg:"
      "*.ws:"
      "*.wsc:"
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
