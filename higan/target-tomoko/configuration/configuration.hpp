struct Settings : Markup::Node {
  Settings();
  auto quit() -> void;
};

extern Settings settings;
