static Settings settings;

void Settings::load() {
  attach(frameGeometryX = 0, "frameGeometryX");
  attach(frameGeometryY = 0, "frameGeometryY");
  attach(frameGeometryWidth = 0, "frameGeometryWidth");
  attach(frameGeometryHeight = 0, "frameGeometryHeight");

  string path = { userpath(), ".config/phoenix/qt.cfg" };
  configuration::load(path);
}

void Settings::save() {
  string path = { userpath(), ".config/" };
  mkdir(path, 0755);
  path.append("phoenix/");
  mkdir(path, 0755);
  path.append("qt.cfg");
  configuration::save(path);
}
