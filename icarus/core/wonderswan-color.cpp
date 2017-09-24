auto Icarus::wonderSwanColorManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return wonderSwanColorManifest(buffer, location);
}

auto Icarus::wonderSwanColorManifest(vector<uint8_t>& buffer, string location) -> string {
  string manifest;

  if(settings["icarus/UseDatabase"].boolean() && !manifest) {
    string digest = Hash::SHA256(buffer.data(), buffer.size()).digest();
    for(auto node : database.wonderSwanColor) {
      if(node["sha256"].text() == digest) {
        manifest.append(node.text(), "\n  sha256: ", digest, "\n");
        break;
      }
    }
  }

  if(settings["icarus/UseHeuristics"].boolean() && !manifest) {
    WonderSwanCartridge cartridge{location, buffer.data(), buffer.size()};
    manifest = cartridge.manifest;
  }

  return manifest;
}

auto Icarus::wonderSwanColorImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "WonderSwan Color/", name, ".wsc/"};

  auto manifest = wonderSwanColorManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");
  if(exists({source, name, ".sav"}) && !exists({target, "save.ram"})) {
    copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) write({target, "manifest.bml"}, manifest);
  write({target, "program.rom"}, buffer);
  return success(target);
}
