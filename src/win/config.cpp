#define __config_add(name, def, type) add(&name, #name, def, type)

class Config : public config {
public:

struct {
  uint32 enabled;
}apu;

struct {
  uint32 mode;
  uint32 use_vram;
  uint32 color_curve;
  uint32 vblank;
}video;

struct {
  struct {
    uint32 up, down, left, right;
    uint32 a, b, x, y, l, r;
    uint32 select, start;
  }joypad1;
}input;

struct {
  uint32 show_fps;
}gui;

  Config() {
    __config_add(apu.enabled, true, TRUEFALSE);

    __config_add(video.mode,            1, DEC);
    __config_add(video.use_vram,     true, TRUEFALSE);
    __config_add(video.color_curve,  true, ENABLED);
    __config_add(video.vblank,      false, TRUEFALSE);

    __config_add(input.joypad1.up,     VK_UP,     HEX);
    __config_add(input.joypad1.down,   VK_DOWN,   HEX);
    __config_add(input.joypad1.left,   VK_LEFT,   HEX);
    __config_add(input.joypad1.right,  VK_RIGHT,  HEX);
    __config_add(input.joypad1.a,      'X',       HEX);
    __config_add(input.joypad1.b,      'Z',       HEX);
    __config_add(input.joypad1.x,      'S',       HEX);
    __config_add(input.joypad1.y,      'A',       HEX);
    __config_add(input.joypad1.l,      'D',       HEX);
    __config_add(input.joypad1.r,      'C',       HEX);
    __config_add(input.joypad1.select, VK_SHIFT,  HEX);
    __config_add(input.joypad1.start,  VK_RETURN, HEX);

    __config_add(gui.show_fps, true, TRUEFALSE);
  }
}cfg;
