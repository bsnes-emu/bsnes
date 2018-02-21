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

  auto document = BML::unserialize(manifest);
  uint prgrom = document["game/memory[name=program.rom]"]["size"].natural();
  uint chrrom = document["game/memory[name=character.rom]"]["size"].natural();

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "ines.rom"}, &buffer[0], 16);
  write({target, "program.rom"}, &buffer[16], prgrom);
  if(!chrrom) return success(target);
  write({target, "character.rom"}, &buffer[16 + prgrom], chrrom);
  return success(target);
}
