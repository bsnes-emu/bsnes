struct Settings : Markup::Node {
  Settings();
  ~Settings();
};

Settings::Settings() {
  Markup::Node::operator=(BML::unserialize(string::read(locate({configpath(), "icarus/"}, "settings.bml"))));

  auto set = [&](const string& name, const string& value) {
    //create node and set to default value only if it does not already exist
    if(!operator[](name)) operator()(name).setValue(value);
  };

  set("Library/Location", {userpath(), "Emulation/"});

  set("icarus/Path", userpath());
  set("icarus/CreateManifests", false);
  set("icarus/UseDatabase", true);
  set("icarus/UseHeuristics", true);
}

Settings::~Settings() {
  file::write(locate({configpath(), "icarus/"}, "settings.bml"), BML::serialize(*this));
}
