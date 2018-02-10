auto Icarus::bsMemoryManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return bsMemoryManifest(buffer, location);
}

auto Icarus::bsMemoryManifest(vector<uint8_t>& buffer, string location) -> string {
  string digest = Hash::SHA256(buffer).digest();

  if(settings["icarus/UseDatabase"].boolean()) {
    for(auto game : database.bsMemory.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings["icarus/UseHeuristics"].boolean()) {
    Heuristics::BSMemory game{buffer.data(), buffer.size()};
    if(string manifest = game.manifest()) return manifest;
  }

  return "";
}

auto Icarus::bsMemoryImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "BS Memory/", name, ".bs/"};

  auto manifest = bsMemoryManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "program.rom"}, buffer);
  return success(target);
}
