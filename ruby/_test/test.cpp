#include <nall/foreach.hpp>
#include <nall/string.hpp>
using namespace nall;

#include <ruby/ruby.cpp>
using namespace ruby;

#include <conio.h>

int main() {
  CoInitialize(0);

  audio.driver("XAudio2");
  audio.set(Audio::Handle, (uintptr_t)GetDesktopWindow());
  audio.set(Audio::Synchronize, true);
  audio.set(Audio::Frequency, 44100U);
  if(audio.init() == false) {
    printf("Failed to initialize audio driver.\n");
    getch();
    return 0;
  }

  input.driver("DirectInput");
  input.set(Input::Handle, (uintptr_t)GetDesktopWindow());
  if(input.init() == false) {
    printf("Failed to initialize input driver.\n");
    getch();
    return 0;
  }

  while(true) {
    int16_t table[Scancode::Limit];
    input.poll(table);
    for(unsigned i = 0; i < Scancode::Limit; i++) {
    //if(table[i]) printf("%.4x\n", i);
    }
  }

  return 0;
}
