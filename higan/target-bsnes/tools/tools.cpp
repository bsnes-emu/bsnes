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
  panel.onChange([&] {
    uint offset = panel.selected().offset();
    if(offset != 0) cheatDatabase.setVisible(false), cheatWindow.setVisible(false);
    if(offset != 1) stateWindow.setVisible(false);
  });

  setTitle("Tools");
  setSize({720, 480});
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
  } else {
    Application::processEvents();
    doSize();
  }
  return *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index).setSelected();
  setVisible();
  setFocused();
}
