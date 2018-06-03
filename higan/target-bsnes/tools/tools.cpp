#include "../bsnes.hpp"
#include "cheat-database.cpp"
#include "cheat-editor.cpp"
unique_pointer<CheatDatabase> cheatDatabase;
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
}

auto ToolsWindow::setVisible(bool visible) -> ToolsWindow& {
  return Window::setVisible(visible), *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
