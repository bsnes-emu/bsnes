struct Settings : Markup::Node {
  Settings();
  auto save() -> void;
};

extern Settings settings;
