static Settings *settings = nullptr;

void Settings::load() {
  string path = { userpath(), ".config/phoenix/gtk.cfg" };
  configuration::load(path);
}

void Settings::save() {
  string path = { userpath(), ".config/" };
  mkdir(path, 0755);
  path.append("phoenix/");
  mkdir(path, 0755);
  path.append("gtk.cfg");
  configuration::save(path);
}

Settings::Settings() {
  attach(frameGeometryX = 4, "frameGeometryX");
  attach(frameGeometryY = 24, "frameGeometryY");
  attach(frameGeometryWidth = 8, "frameGeometryWidth");
  attach(frameGeometryHeight = 28, "frameGeometryHeight");
  attach(menuGeometryHeight = 20, "menuGeometryHeight");
  attach(statusGeometryHeight = 20, "statusGeometryHeight");
}
