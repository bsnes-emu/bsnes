struct Settings : Configuration::Document {
  string path;
  string geometry;

  Settings() {
    Configuration::Node node;
    node.append(path = userpath(), "Path");
    node.append(geometry = "64,64,480,600", "Geometry");
    append(node, "Settings");
    directory::create({configpath(), "ananke/"});
    load({configpath(), "ananke/settings.bml"});
  }

  ~Settings() {
    save({configpath(), "ananke/settings.bml"});
  }
} config;
