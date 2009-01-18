namespace event {

#include "debugger.hpp"

void input_event(uint16_t);

void acquire();
void unacquire();

struct VideoSettings {
  unsigned mode;
  bool synchronize;
  bool aspect_correction;
  unsigned region;
  unsigned multiplier;
  unsigned hardware_filter;
  unsigned software_filter;
} video_settings;
void load_video_settings();

void update_aspect_correction(bool);
void update_multiplier(unsigned);
void update_region(unsigned);
void update_hardware_filter(unsigned);
void update_software_filter(unsigned);

void update_frameskip(int);
void update_emulation_speed(int);

enum system_state_t { LoadCart, UnloadCart, PowerOn, PowerOff, PowerCycle, Reset };
void modify_system_state(system_state_t);

void update_controller_port1(int);
void update_controller_port2(int);

void update_video_settings();
void update_opacity();

void toggle_fullscreen();
void toggle_menubar();
void toggle_statusbar();

bool load_cart(char*);
void load_cart();
void load_image(const char*);
void load_cart_normal(const char*);
void load_cart_bsc(const char*, const char*);
void load_cart_bsx(const char*, const char*);
void load_cart_st(const char*, const char*, const char*);
void unload_cart();

void quit();

} //namespace event
