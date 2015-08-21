#include <nall/nall.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

#include "settings.cpp"
Settings settings;

#include "heuristics/famicom.hpp"
#include "heuristics/super-famicom.hpp"
#include "heuristics/game-boy.hpp"
#include "heuristics/game-boy-advance.hpp"
#include "heuristics/bsx-satellaview.hpp"
#include "heuristics/sufami-turbo.hpp"

#include "core/core.hpp"
#include "core/core.cpp"
#include "core/famicom.cpp"
#include "core/super-famicom.cpp"
#include "core/game-boy.cpp"
#include "core/game-boy-color.cpp"
#include "core/game-boy-advance.cpp"
#include "core/bsx-satellaview.cpp"
#include "core/sufami-turbo.cpp"
Icarus icarus;

#include "ui/ui.hpp"
#include "ui/scan-dialog.cpp"
#include "ui/import-dialog.cpp"
#include "ui/error-dialog.cpp"

#include <nall/main.hpp>
auto nall::main(lstring args) -> void {
  if(args.size() == 3 && args[1] == "-m") {
    if(!directory::exists(args[2])) return print("error: directory not found\n");
    return print(icarus.manifest(args[2]));
  }

  new ScanDialog;
  new ImportDialog;
  new ErrorDialog;
  scanDialog->show();
  Application::run();
}
