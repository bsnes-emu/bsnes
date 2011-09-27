#include "../base.hpp"
WindowManager *windowManager = 0;

void WindowManager::append(Window *window, const string &name) {
  windowList.append({ window, name, geometry(window->geometry()) });

  window->setMenuFont(application->normalFont);
  window->setWidgetFont(application->normalFont);
  window->setStatusFont(application->boldFont);
}

string WindowManager::geometry(const Geometry &geometry) {
  return { geometry.x, ",", geometry.y, ",", geometry.width, ",", geometry.height };
}

Geometry WindowManager::geometry(const string &geometry) {
  lstring part;
  part.split(",", geometry);
  Geometry geom = {
      (signed)integer(part[0]),   (signed)integer(part[1]),
    (unsigned)decimal(part[2]), (unsigned)decimal(part[3])
  };
  geom.x = max(0, min(7680, geom.x));
  geom.y = max(0, min(4800, geom.y));
  geom.width  = min(7680, geom.width );
  geom.height = min(4800, geom.height);
  return geom;
}

void WindowManager::loadGeometry() {
  for(auto &window : windowList) {
    config.attach(window.geometry, window.name);
  }
  config.load(string{ application->userpath, "geometry.cfg" });
  config.save(string{ application->userpath, "geometry.cfg" });
  for(auto &window : windowList) {
    window.window->setGeometry(geometry(window.geometry));
  }
}

void WindowManager::saveGeometry() {
  for(auto &window : windowList) {
    window.geometry = geometry(window.window->geometry());
  }
  config.save(string{ application->userpath, "geometry.cfg" });
}
