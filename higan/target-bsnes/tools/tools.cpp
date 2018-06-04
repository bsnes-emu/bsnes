#include "../bsnes.hpp"
#include "cheat-editor.cpp"
unique_pointer<CheatDatabase> cheatDatabase;
unique_pointer<CheatWindow> cheatWindow;
unique_pointer<ToolsWindow> toolsWindow;

ToolsWindow::ToolsWindow() {
  toolsWindow = this;

  layout.setMargin(5);

  setTitle("Tools");
  setSize({600, 400});
  setAlignment({1.0, 1.0});
  setDismissable();

  onSize([&] {
    cheatEditor.cheatList.resizeColumns();
  });

  onClose([&] {
    setVisible(false);
  });
}

auto ToolsWindow::setVisible(bool visible) -> ToolsWindow& {
  Window::setVisible(visible);
  if(!visible) {
    cheatDatabase->setVisible(false);
    cheatWindow->setVisible(false);
  }
  return *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
