#include "../bsnes.hpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
#include "manifest-viewer.cpp"
CheatDatabase cheatDatabase;
CheatWindow cheatWindow;
CheatEditor cheatEditor;
StateWindow stateWindow;
StateManager stateManager;
ManifestViewer manifestViewer;
ToolsWindow toolsWindow;

auto ToolsWindow::create() -> void {
  layout.setPadding(5);
  panel.append(cheatEditor);
  panel.append(stateManager);
  panel.append(manifestViewer);

  setTitle("Tools");
  setSize({600, 400});
  setAlignment({1.0, 1.0});
  setDismissable();

  onSize([&] {
    cheatEditor.cheatList.resizeColumns();
    stateManager.stateList.resizeColumns();
  });

  onClose([&] {
    setVisible(false);
  });
}

auto ToolsWindow::setVisible(bool visible) -> ToolsWindow& {
  Window::setVisible(visible);
  if(!visible) {
    cheatDatabase.setVisible(false);
    cheatWindow.setVisible(false);
    stateWindow.setVisible(false);
  }
  return *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index).setSelected();
  setVisible();
  setFocused();
  doSize();
}
