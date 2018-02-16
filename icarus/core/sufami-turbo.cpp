auto Icarus::sufamiTurboManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return sufamiTurboManifest(buffer, location);
}

auto Icarus::sufamiTurboManifest(vector<uint8_t>& buffer, string location) -> string {
  auto digest = Hash::SHA256(buffer).digest();

  if(settings["icarus/UseDatabase"].boolean()) {
    for(auto game : database.sufamiTurbo.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings["icarus/UseHeuristics"].boolean()) {
    Heuristics::SufamiTurbo game{buffer, location};
    if(auto manifest = game.manifest()) return manifest;
  }

  return {};
}

auto Icarus::sufamiTurboImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "Sufami Turbo/", name, ".st/"};

  auto manifest = sufamiTurboManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "program.rom"}, buffer);
  return success(target);
}
