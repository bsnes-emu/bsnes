namespace phoenix {

static Settings *settings = nullptr;

void Settings::load() {
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

Settings::Settings() {
  append(frameGeometryX = 4, "frameGeometryX");
  append(frameGeometryY = 24, "frameGeometryY");
  append(frameGeometryWidth = 8, "frameGeometryWidth");
  append(frameGeometryHeight = 28, "frameGeometryHeight");
  append(menuGeometryHeight = 20, "menuGeometryHeight");
  append(statusGeometryHeight = 20, "statusGeometryHeight");
}

}
