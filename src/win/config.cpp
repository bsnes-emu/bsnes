#define __config_add(name, def, type) add(&name, #name, def, type)

class Config : public config {
public:

struct {
  uint32 mode;
  uint32 use_vram;
}video;

  Config() {
    __config_add(video.mode,        1, DEC);
    __config_add(video.use_vram, true, TRUEFALSE);
  }
}cfg;
