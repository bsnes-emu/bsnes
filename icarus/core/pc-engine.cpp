auto Icarus::pcEngineManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return pcEngineManifest(buffer, location);
}

auto Icarus::pcEngineManifest(vector<uint8_t>& buffer, string location) -> string {
  string manifest;

  if(settings["icarus/UseDatabase"].boolean() && !manifest) {
    string digest = Hash::SHA256(buffer.data(), buffer.size()).digest();
    for(auto node : database.pcEngine) {
      if(node["sha256"].text() == digest) {
        manifest.append(node.text(), "\n  sha256: ", digest, "\n");
        break;
      }
    }
  }

  if(settings["icarus/UseHeuristics"].boolean() && !manifest) {
    PCEngineCartridge cartridge{location, buffer.data(), buffer.size()};
    manifest = cartridge.manifest;
  }

  return manifest;
}

auto Icarus::pcEngineImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "PC Engine/", name, ".pce/"};

  auto manifest = pcEngineManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "program.rom"}, buffer);
  return success(target);
}
