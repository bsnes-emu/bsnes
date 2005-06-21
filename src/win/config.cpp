#define __config_add(name, def, type) add(&name, #name, def, type)

class Config : public config {
public:

struct {
  uint32 mode;
  uint32 use_vram;
}video;

struct {
  uint32 show_fps;
}gui;

  Config() {
    __config_add(video.mode,        1, DEC);
    __config_add(video.use_vram, true, TRUEFALSE);
    __config_add(gui.show_fps,   true, TRUEFALSE);
  }
}cfg;
