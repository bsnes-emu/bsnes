namespace hiro {

struct Settings : Configuration::Document {
  vector<uint16_t> keycodes;

  struct Geometry : Configuration::Node {
    signed frameX;
    signed frameY;
    signed frameWidth;
    signed frameHeight;
    signed menuHeight;
    signed statusHeight;
  } geometry;

  Settings();
  auto load() -> void;
  auto save() -> void;
};

}
