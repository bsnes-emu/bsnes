#include "../higan.hpp"
#include "cheat-database.cpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
Tools* tools = nullptr;

ToolsLayout::ToolsLayout() {
  setMargin(5);
}

Tools::Tools() {
  setGeometry({128, 128, 640, 360});
  windowManager->append(this, "Tools");

  setTitle("Tools");

  layout.setMargin(5);
  panels.append("Cheat Editor", {resource::cheatEditor, sizeof resource::cheatEditor});
  panels.append("State Manager", {resource::stateManager, sizeof resource::stateManager});
  panels.setLayout(0, *cheatEditor);
  panels.setLayout(1, *stateManager);

  append(layout);
  layout.append(panels, {~0, ~0});
}
