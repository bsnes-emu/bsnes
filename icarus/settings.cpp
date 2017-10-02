struct Settings : Markup::Node {
  Settings();
  auto save() -> void;
};

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate("settings.bml"))));

  auto set = [&](const string& name, const string& value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Library/Location", {Path::user(), "Emulation/"});

  set("icarus/Path", Path::user());
  set("icarus/CreateManifests", false);
  set("icarus/UseDatabase", true);
  set("icarus/UseHeuristics", true);
}

auto Settings::save() -> void {
  file::write(locate("settings.bml"), BML::serialize(*this));
}
