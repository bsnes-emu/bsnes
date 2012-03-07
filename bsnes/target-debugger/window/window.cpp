#include "../base.hpp"
WindowManager *windowManager = nullptr;

void WindowManager::append(Window *window, const string &name) {
  windowList.append({ window, name, window->geometry().text() });
}

void WindowManager::loadGeometry() {
  for(auto &window : windowList) {
    config.append(window.geometry, window.name);
  }
  config.load({ application->userpath, "geometry.cfg" });
  config.save({ application->userpath, "geometry.cfg" });
  for(auto &window : windowList) {
    window.window->setGeometry(window.geometry);
  }
}

void WindowManager::saveGeometry() {
  for(auto &window : windowList) {
    window.geometry = window.window->geometry().text();
    window.window->setVisible(false);
  }
  config.save({ application->userpath, "geometry.cfg" });
}
