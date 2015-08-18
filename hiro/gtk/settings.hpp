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

  struct Window : Configuration::Node {
    unsigned backgroundColor;
  } window;

  Settings();
  auto load() -> void;
  auto save() -> void;
};

static Settings* settings = nullptr;

}
