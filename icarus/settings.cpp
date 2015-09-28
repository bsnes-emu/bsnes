struct Settings : Configuration::Document {
  Settings();
  ~Settings();

  Configuration::Node root;
  string activePath;
  string libraryPath;
  bool createManifests = false;
  bool useDatabase = true;
  bool useHeuristics = true;
};

Settings::Settings() {
  root.append(activePath, "ActivePath");
  root.append(libraryPath, "LibraryPath");
  root.append(createManifests, "CreateManifests");
  root.append(useDatabase, "UseDatabase");
  root.append(useHeuristics, "UseHeuristics");
  append(root, "Settings");

  directory::create({configpath(), "icarus/"});
  load({configpath(), "icarus/settings.bml"});
  save({configpath(), "icarus/settings.bml"});

  if(!activePath) activePath = userpath();
  if(!libraryPath) libraryPath = {userpath(), "Emulation/"};
}

Settings::~Settings() {
  save({configpath(), "icarus/settings.bml"});
}
