void run();
bool _term_ = false;

#include "ui.h"
#include "event.h"

#include "ui.cpp"
#include "event.cpp"

void alert(const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
#if defined(PLATFORM_WIN)
  MessageBox(0, str, "bsnes", MB_OK);
#else
  fprintf(stdout, "%s\r\n", str);
#endif
}

void dprintf(const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "%s\r\n", str);
}

void dprintf(uint source, const char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  fprintf(stdout, "[%d]: %s\r\n", source, str);
}

void set_config_filename() {
  userpath(config::filename);
  strcat(config::filename, "/.bsnes");
  mkdir(config::filename); //always make directory in case it does not exist, fail silently if it does
  strcat(config::filename, "/bsnes.cfg");
}

void get_base_path(const char *image) {
char full_name[PATH_MAX] = "";

#if defined(PLATFORM_WIN)
  GetFullPathName(image, PATH_MAX, full_name, 0);
#elif defined(PLATFORM_X)
  realpath(image, full_name);
#endif

string t = full_name;
  if(strlen(t) != 0) {
  //remove program name
  //TODO: rewrite this to be cleaner ...
    replace(t, "\\", "/");
    for(int i = strlen(t) - 1; i >= 0; i--) {
      if(t()[i] == '/' || t()[i] == '\\') {
        t()[i] = 0;
        break;
      }
    }
  }

  if(strend(t, "/") == false) { strcat(t, "/"); }
  config::path.base = t;
}

void run() {
  while(miu().pending()) miu().run();

  if(cartridge.loaded() == true) {
    snes.runtoframe();
    event::update_frame_counter();
  } else { //prevent bsnes from consuming 100% CPU resources when idle
  #if defined(PLATFORM_WIN)
    Sleep(1);
  #elif defined(PLATFORM_X)
    usleep(20);
  #endif
  }
}

#if defined(PLATFORM_WIN)
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
int argc = __argc;
char **argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif

//int main(int argc, char *argv[]) {
  set_config_filename();
  get_base_path(argv[0]);

  miu().init();
  config::config().load(config::filename);
  if(fexists(config::filename) == false) {
  //in case program crashes on first run, save config file
  //settings, so that they can be modified by hand ...
    config::config().save(config::filename);
  }
  snes.init();
  ui_init();

  if(argc >= 2) {
    cartridge.load_cart_normal(argv[1]);
    snes.power();
  }

  while(_term_ == false) {
    run();
  }

  event::unload_rom();

  config::config().save(config::filename);
  snes.term();
  ui_term();
  miu().term();
  return 0;
}
