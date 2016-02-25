#include "../loki.hpp"
#include "about-window.cpp"
unique_pointer<Terminal> terminal;

Terminal::Terminal() {
  terminal = this;
  new AboutWindow;

  onClose([&] { program->quit(); });

  helpMenu.setText("Help");
  aboutAction.setText("About ...").onActivate([&] { aboutWindow->setFocused(); });

  console.setBackgroundColor({ 56,  56,  56});
  console.setForegroundColor({255, 255, 255});
  console.setFont(Font().setFamily(Font::Mono).setSize(8));
  console.setPrompt("$ ");

  setTitle({"loki v", Emulator::Version});
  setSize({800, 480});
  setAlignment({0.0, 1.0});
  setVisible();
}

auto Terminal::showAboutWindow() -> void {
}
