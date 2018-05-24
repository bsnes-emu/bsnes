auto Program::path(string type, string location, string extension) -> string {
  auto pathname = Location::path(location);
  auto filename = Location::file(location);
  auto prefix = Location::prefix(filename);
  auto suffix = Location::suffix(filename);

  if(type == "Games") {
    if(auto path = settings["Path/Games"].text()) {
      pathname = path;
    }
  }

  if(type == "Patches") {
    if(auto path = settings["Path/Patches"].text()) {
      pathname = path;
    }
  }

  if(type == "Saves") {
    if(auto path = settings["Path/Saves"].text()) {
      pathname = path;
    }
  }

  if(type == "States") {
    if(auto path = settings["Path/States"].text()) {
      pathname = path;
    }
  }

  if(type == "Cheats") {
    if(auto path = settings["Path/Cheats"].text()) {
      pathname = path;
    }
  }

  if(extension) {
    suffix = extension;
  }

  return {pathname, prefix, suffix};
}
