#include "../bsnes.hpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
#include "manifest-viewer.cpp"
namespace Instances { Instance<CheatDatabase> cheatDatabase; }
CheatDatabase& cheatDatabase = Instances::cheatDatabase();
namespace Instances { Instance<CheatWindow> cheatWindow; }
CheatWindow& cheatWindow = Instances::cheatWindow();
CheatEditor cheatEditor;
namespace Instances { Instance<StateWindow> stateWindow; }
StateWindow& stateWindow = Instances::stateWindow();
StateManager stateManager;
ManifestViewer manifestViewer;
namespace Instances { Instance<ToolsWindow> toolsWindow; }
ToolsWindow& toolsWindow = Instances::toolsWindow();

auto ToolsWindow::create() -> void {
  layout.setPadding(5_sx);
  panel.append(cheatEditor);
  panel.append(stateManager);
  panel.append(manifestViewer);
  panel.onChange([&] {
    uint offset = panel.selected().offset();
    if(offset != 0) cheatDatabase.setVisible(false), cheatWindow.setVisible(false);
    if(offset != 1) stateWindow.setVisible(false);
  });

  setTitle("Tools");
  setSize({720_sx, 480_sx});
  setAlignment({1.0, 1.0});
  setDismissable();

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
  }
  return *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index).setSelected();
  setVisible();
  setFocused();
}
