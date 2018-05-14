auto Icarus::famicomManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "ines.rom"});
  concatenate(buffer, {location, "program.rom"});
  concatenate(buffer, {location, "character.rom"});
  return famicomManifest(buffer, location);
}

auto Icarus::famicomManifest(vector<uint8_t>& buffer, string location) -> string {
  if(settings["icarus/UseDatabase"].boolean()) {
    auto digest = Hash::SHA256(buffer).digest();
    for(auto game : Database::Famicom.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings["icarus/UseHeuristics"].boolean()) {
    Heuristics::Famicom game{buffer, location};
    if(auto manifest = game.manifest()) return manifest;
  }

  return {};
}

auto Icarus::famicomImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "Famicom/", name, ".fc/"};

  auto manifest = famicomManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  auto document = BML::unserialize(manifest);
  uint offset = 0;
  if(true) {
    //todo: support images without iNES headers (via database lookup)
    uint size = 16;
    write({target, "ines.rom"}, &buffer[offset], size);
    offset += size;
  }
  if(auto program = document["game/memory(type=ROM,content=Program)"]) {
    uint size = program["size"].natural();
    write({target, "program.rom"}, &buffer[offset], size);
    offset += size;
  }
  if(auto character = document["game/memory(type=ROM,content=Character)"]) {
    uint size = character["size"].natural();
    write({target, "character.rom"}, &buffer[offset], size);
    offset += size;
  }
  return success(target);
}
