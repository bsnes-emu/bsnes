#include "../tomoko.hpp"
#include "cheat-database.cpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
CheatDatabase* cheatDatabase = nullptr;
ToolsManager* toolsManager = nullptr;

ToolsManager::ToolsManager() {
  toolsManager = this;

  layout.setMargin(5);

  setTitle("Tools");
  setSize({600, 400});
  setPlacement(1.0, 1.0);

  onSize([&] {
    cheatEditor.cheatList.resizeColumns();
    stateManager.stateList.resizeColumns();
  });
}

auto ToolsManager::show(unsigned tool) -> void {
  panel.item(tool)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
