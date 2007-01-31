#include <SDL.h>

class VideoSDL : public Video {
private:
unsigned long window;
SDL_Surface *screen, *backbuffer;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return 1152; }
  uint screen_height() { return  864; }

  void redraw();
  void update();
  void init();
  void term();

  VideoSDL(unsigned long handle);
};
