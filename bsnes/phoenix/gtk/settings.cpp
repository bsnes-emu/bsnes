struct Settings : public configuration {
  unsigned frameGeometryX;
  unsigned frameGeometryY;
  unsigned frameGeometryWidth;
  unsigned frameGeometryHeight;

  void load() {
    string path = { userpath(), ".config/phoenix/gtk.cfg" };
    configuration::load(path);
  }

  void save() {
    string path = { userpath(), ".config/" };
    mkdir(path, 0755);
    path.append("phoenix/");
    mkdir(path, 0755);
    path.append("gtk.cfg");
    configuration::save(path);
  }

  Settings() {
    attach(frameGeometryX = 0, "frameGeometryX");
    attach(frameGeometryY = 0, "frameGeometryY");
    attach(frameGeometryWidth = 0, "frameGeometryWidth");
    attach(frameGeometryHeight = 0, "frameGeometryHeight");
  }
};

static Settings settings;
