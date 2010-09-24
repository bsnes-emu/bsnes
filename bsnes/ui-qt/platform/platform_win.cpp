#include <nall/utf8.hpp>

bool Application::App::winEventFilter(MSG *msg, long *result) {
  //supress screen saver from activating during gameplay
  if(msg->message == WM_SYSCOMMAND) {
    if(msg->wParam == SC_SCREENSAVE || msg->wParam == SC_MONITORPOWER) {
      *result = 0;
      return true;
    }
  }

  //prevent DirectSound audio buffer from looping during Windows modal events
  if(msg->message == WM_ENTERMENULOOP || msg->message == WM_ENTERSIZEMOVE) {
    audio.clear();
  }

  return false;
}

void supressScreenSaver() {
  //handled by event filter above
}

