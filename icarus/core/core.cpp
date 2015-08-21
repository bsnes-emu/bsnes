auto Icarus::error() const -> string {
  return errorMessage;
}

auto Icarus::success() -> bool {
  errorMessage = "";
  return true;
}

auto Icarus::failure(const string& message) -> bool {
  errorMessage = message;
  return false;
}

auto Icarus::manifest(string location) -> string {
  location.transform("\\", "/").rtrim("/").append("/");
  if(!directory::exists(location)) return "";

  auto type = location.suffixname().downcase();
  if(type == ".fc") return famicomManifest(location);
  if(type == ".sfc") return superFamicomManifest(location);
  if(type == ".gb") return gameBoyManifest(location);
  if(type == ".gbc") return gameBoyColorManifest(location);
  if(type == ".gba") return gameBoyAdvanceManifest(location);
  if(type == ".bs") return bsxSatellaviewManifest(location);
  if(type == ".st") return sufamiTurboManifest(location);

  return "";
}

auto Icarus::import(string location) -> bool {
  location.transform("\\", "/").rtrim("/");
  if(!file::exists(location)) return failure("file does not exist");
  if(!file::readable(location)) return failure("file is unreadable");

  auto name = location.prefixname();
  auto type = location.suffixname().downcase();
  if(!name || !type) return failure("invalid file name");

  auto buffer = file::read(location);
  if(!buffer) return failure("file is empty");

  if(type == ".zip") {
    Decode::ZIP zip;
    if(!zip.open(location)) return failure("ZIP archive is invalid");
    if(!zip.file) return failure("ZIP archive is empty");

    name = zip.file[0].name.prefixname();
    type = zip.file[0].name.suffixname().downcase();
    buffer = zip.extract(zip.file[0]);
  }

  if(type == ".fc" || type == ".nes") return famicomImport(buffer, location);
  if(type == ".sfc" || type == ".smc") return superFamicomImport(buffer, location);
  if(type == ".gb") return gameBoyImport(buffer, location);
  if(type == ".gbc") return gameBoyColorImport(buffer, location);
  if(type == ".gba") return gameBoyAdvanceImport(buffer, location);
  if(type == ".bs") return bsxSatellaviewImport(buffer, location);
  if(type == ".st") return sufamiTurboImport(buffer, location);

  return failure("unrecognized file extension");
}

auto Icarus::concatenate(vector<uint8_t>& output, const string& location) -> void {
  if(auto input = file::read(location)) {
    auto size = output.size();
    output.resize(size + input.size());
    memory::copy(output.data() + size, input.data(), input.size());
  }
}
