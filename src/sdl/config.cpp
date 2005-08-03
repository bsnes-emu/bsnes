#define __config_add(name, def, type) add(&name, #name, def, type)

class Config : public config {
public:

struct {
  uint32 enabled;
}apu;

struct {
  uint32 fullscreen;
  uint32 display_width, display_height;
  uint32 output_width,  output_height;
}video;

struct {
  struct {
    uint32 up, down, left, right;
    uint32 a, b, x, y, l, r;
    uint32 select, start;
  }joypad1;
}input;

  Config() {
    __config_add(apu.enabled, true, TRUEFALSE);

    __config_add(video.fullscreen, false, TRUEFALSE);
    __config_add(video.display_width,  256, DEC);
    __config_add(video.display_height, 223, DEC);
    __config_add(video.output_width,   256, DEC);
    __config_add(video.output_height,  223, DEC);

    __config_add(input.joypad1.up,     SDLK_UP,     HEX);
    __config_add(input.joypad1.down,   SDLK_DOWN,   HEX);
    __config_add(input.joypad1.left,   SDLK_LEFT,   HEX);
    __config_add(input.joypad1.right,  SDLK_RIGHT,  HEX);
    __config_add(input.joypad1.a,      SDLK_x,      HEX);
    __config_add(input.joypad1.b,      SDLK_z,      HEX);
    __config_add(input.joypad1.x,      SDLK_s,      HEX);
    __config_add(input.joypad1.y,      SDLK_a,      HEX);
    __config_add(input.joypad1.l,      SDLK_d,      HEX);
    __config_add(input.joypad1.r,      SDLK_c,      HEX);
    __config_add(input.joypad1.select, SDLK_RSHIFT, HEX);
    __config_add(input.joypad1.start,  SDLK_RETURN, HEX);
  }

}cfg;
