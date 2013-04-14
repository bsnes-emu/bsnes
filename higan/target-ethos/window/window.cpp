#include "../ethos.hpp"
WindowManager *windowManager = nullptr;

void WindowManager::append(Window *window, const string &name) {
  window->setMenuFont(program->normalFont);
  window->setWidgetFont(program->normalFont);
  window->setStatusFont(program->boldFont);
  windowList.append({window, name, window->geometry().text()});
}

void WindowManager::loadGeometry() {
  static bool initialized = false;
  if(initialized == false) {
    initialized = true;
    Configuration::Node geometry;
    for(auto &window : windowList) {
      geometry.append(window.geometry, window.name);
    }
    config.append(geometry, "Geometry");
  }
  config.load(program->path("geometry.bml"));
  config.save(program->path("geometry.bml"));
  for(auto &window : windowList) {
    window.window->setGeometry(window.geometry);
  }
}

void WindowManager::saveGeometry() {
  for(auto &window : windowList) {
    window.geometry = window.window->geometry().text();
  }
  config.save(program->path("geometry.bml"));
}

void WindowManager::hideAll() {
  for(auto &window : windowList) {
    window.window->setVisible(false);
  }
}
