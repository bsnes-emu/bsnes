namespace hiro {

struct Settings {
  Settings();
  ~Settings();

  vector<uint16_t> keycodes;

  struct Geometry {
    int frameX = 4;
    int frameY = 24;
    int frameWidth = 8;
    int frameHeight = 28;
    int menuHeight = 20;
    int statusHeight = 20;
  } geometry;
};

static Settings settings;

}
