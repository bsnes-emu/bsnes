#define __config_add(name, def, type) add(&name, #name, def, type)

class Config : public config {
public:

struct {
  uint32 mode;
  uint32 use_vram;
  uint32 color_curve;
  uint32 vblank;
}video;

struct {
  uint32 show_fps;
}gui;

  Config() {
    __config_add(video.mode,            1, DEC);
    __config_add(video.use_vram,     true, TRUEFALSE);
    __config_add(video.color_curve,  true, ENABLED);
    __config_add(video.vblank,      false, TRUEFALSE);
    __config_add(gui.show_fps,       true, TRUEFALSE);
  }
}cfg;
