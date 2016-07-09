auto Icarus::megaDriveManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.rom"});
  return megaDriveManifest(buffer, location);
}

auto Icarus::megaDriveManifest(vector<uint8_t>& buffer, string location) -> string {
  string manifest;

  if(settings["icarus/UseDatabase"].boolean() && !manifest) {
    string digest = Hash::SHA256(buffer.data(), buffer.size()).digest();
    for(auto node : database.megaDrive) {
      if(node["sha256"].text() == digest) {
        manifest.append(node.text(), "\n  sha256: ", digest, "\n");
        break;
      }
    }
  }

  if(settings["icarus/UseHeuristics"].boolean() && !manifest) {
    MegaDriveCartridge cartridge{location, buffer.data(), buffer.size()};
    manifest = cartridge.manifest;
  }

  return manifest;
}

auto Icarus::megaDriveImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings["Library/Location"].text(), "Mega Drive/", name, ".md/"};
//if(directory::exists(target)) return failure("game already exists");

  auto manifest = megaDriveManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!directory::create(target)) return failure("library path unwritable");
  if(file::exists({source, name, ".sav"}) && !file::exists({target, "save.ram"})) {
    file::copy({source, name, ".sav"}, {target, "save.ram"});
  }

  if(settings["icarus/CreateManifests"].boolean()) file::write({target, "manifest.bml"}, manifest);
  file::write({target, "program.rom"}, buffer);
  return success(target);
}
