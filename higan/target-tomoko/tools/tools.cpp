#include "../tomoko.hpp"
#include "cheat-database.cpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
#include "manifest-viewer.cpp"
#include "game-notes.cpp"
unique_pointer<CheatDatabase> cheatDatabase;
unique_pointer<ToolsManager> toolsManager;

ToolsManager::ToolsManager() {
  toolsManager = this;

  layout.setMargin(5);

  setTitle("Tools");
  setSize({600, 405});
  setAlignment({1.0, 1.0});
  setDismissable();

  onSize([&] {
    cheatEditor.cheatList.resizeColumns();
    stateManager.stateList.resizeColumns();
  });
}

auto ToolsManager::show(uint tool) -> void {
  panel.item(tool)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
