auto Program::path(string type, string location, string extension) -> string {
  auto pathname = Location::path(location);
  auto filename = Location::file(location);
  auto prefix = Location::prefix(filename);
  auto suffix = extension;

  if(type == "Games") {
    if(auto path = settings.path.games) pathname = path;
  }

  if(type == "Patches") {
    if(auto path = settings.path.patches) pathname = path;
  }

  if(type == "Saves") {
    if(auto path = settings.path.saves) pathname = path;
  }

  if(type == "Cheats") {
    if(auto path = settings.path.cheats) pathname = path;
  }

  if(type == "States") {
    if(auto path = settings.path.states) pathname = path;
  }

  if(type == "Screenshots") {
    if(auto path = settings.path.screenshots) pathname = path;
  }

  return {pathname, prefix, suffix};
}

auto Program::gamePath() -> string {
  if(!emulator->loaded()) return "";
  if(gameBoy.location) return gameBoy.location;
  return superFamicom.location;
}

auto Program::cheatPath() -> string {
  if(!emulator->loaded()) return "";
  auto location = gamePath();
  if(location.endsWith("/")) {
    return {location, "cheats.bml"};
  } else {
    return path("Cheats", location, ".cht");
  }
}

auto Program::statePath() -> string {
  if(!emulator->loaded()) return "";
  auto location = gamePath();
  if(location.endsWith("/")) {
    return {location, "bsnes/states/"};
  } else {
    return path("States", location, ".bsz");
  }
}

auto Program::screenshotPath() -> string {
  if(!emulator->loaded()) return "";
  auto location = gamePath();
  if(location.endsWith("/")) {
    location = {location, "bsnes/screenshots/"};
    directory::create(location);
  } else {
    location = {path("Screenshots", location), "-"};
  }
  for(uint n : range(1, 1000)) {
    string filename = {location, pad(n, 3, '0'), ".bmp"};
    if(!file::exists(filename)) return filename;
  }
  return {location, "000.bmp"};
}
