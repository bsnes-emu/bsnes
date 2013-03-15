#include "../ethos.hpp"
WindowManager *windowManager = nullptr;

void WindowManager::append(Window *window, const string &name) {
  window->setMenuFont(program->normalFont);
  window->setWidgetFont(program->normalFont);
  window->setStatusFont(program->boldFont);
  windowList.append({window, name, window->geometry().text()});
}

void WindowManager::loadGeometry() {
  for(auto &window : windowList) {
    config.append(window.geometry, window.name);
  }
  config.load(program->path("geometry.cfg"));
  config.save(program->path("geometry.cfg"));
  for(auto &window : windowList) {
    window.window->setGeometry(window.geometry);
  }
}

void WindowManager::saveGeometry() {
  for(auto &window : windowList) {
    window.geometry = window.window->geometry().text();
  }
  config.save(program->path("geometry.cfg"));
}

void WindowManager::hideAll() {
  for(auto &window : windowList) {
    window.window->setVisible(false);
  }
}
