#include <SDL.h>

class VideoSDL : public Video {
private:
void *window;
SDL_Surface *screen, *backbuffer;

public:
  uint16 *lock(uint &pitch);
  void    unlock();

  uint    screen_width()  { return 1152; }
  uint    screen_height() { return  864; }

  void    redraw();
  void    update();
  void    init();
  void    term();

  VideoSDL(void *handle = 0);
};
