namespace phoenix {

static Settings *settings = nullptr;

void Settings::load() {
  string path = {userpath(), ".config/phoenix/gtk.bml"};
  Configuration::Document::load(path);
}

void Settings::save() {
  string path = {userpath(), ".config/phoenix/"};
  directory::create(path, 0755);
  path.append("gtk.bml");
  Configuration::Document::save(path);
}

Settings::Settings() {
  geometry.append(geometry.frameX = 4, "FrameX");
  geometry.append(geometry.frameY = 24, "FrameY");
  geometry.append(geometry.frameWidth = 8, "FrameWidth");
  geometry.append(geometry.frameHeight = 28, "FrameHeight");
  geometry.append(geometry.menuHeight = 20, "MenuHeight");
  geometry.append(geometry.statusHeight = 20, "StatusHeight");
  append(geometry, "Geometry");
  window.append(window.backgroundColor = 0xedeceb, "BackgroundColor");
  append(window, "Window");
}

}
