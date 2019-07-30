#include "../bsnes.hpp"
#include "cheat-finder.cpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
#include "manifest-viewer.cpp"
namespace Instances { Instance<CheatDatabase> cheatDatabase; }
CheatFinder cheatFinder;
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

struct ToolsHome : VerticalLayout {
  ToolsHome() {
    setCollapsible();
    setVisible(false);
    image icon{Resource::Icon};
    auto data = icon.data();
    for(uint y : range(icon.height())) {
      for(uint x : range(icon.width())) {
        auto pixel = icon.read(data);
        auto a = pixel >> 24 & 255;
        auto r = pixel >> 16 & 255;
        auto g = pixel >>  8 & 255;
        auto b = pixel >>  0 & 255;
        a = a * 0.25;
        icon.write(data, a << 24 | r << 16 | g << 8 | b << 0);
        data += icon.stride();
      }
    }
    canvas.setIcon(icon);
  }

public:
  Canvas canvas{this, Size{~0, ~0}};
} toolsHome;

auto ToolsWindow::create() -> void {
  layout.setPadding(5_sx);
  panelList.append(ListViewItem().setText("Cheat Finder").setIcon(Icon::Action::Search));
  panelList.append(ListViewItem().setText("Cheat Editor").setIcon(Icon::Edit::Replace));
  panelList.append(ListViewItem().setText("State Manager").setIcon(Icon::Application::FileManager));
  panelList.append(ListViewItem().setText("Manifest Viewer").setIcon(Icon::Emblem::Text));
  panelList.onChange([&] {
    if(auto item = panelList.selected()) {
      show(item.offset());
    } else {
      show(-1);
    }
  });
  panelContainer.append(toolsHome, Size{~0, ~0});
  panelContainer.append(cheatFinder, Size{~0, ~0});
  panelContainer.append(cheatEditor, Size{~0, ~0});
  panelContainer.append(stateManager, Size{~0, ~0});
  panelContainer.append(manifestViewer, Size{~0, ~0});

  setTitle("Tools");
  setSize({720_sx, 400_sx});
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

auto ToolsWindow::show(int index) -> void {
  toolsHome.setVisible(false);
  cheatFinder.setVisible(false);
  cheatEditor.setVisible(false);
  stateManager.setVisible(false);
  manifestViewer.setVisible(false);
  panelList.item(index).setSelected();
  if(index ==-1) toolsHome.setVisible(true);
  if(index == 0) cheatFinder.setVisible(true);
  if(index == 1) cheatEditor.setVisible(true);
  if(index == 2) stateManager.setVisible(true);
  if(index == 3) manifestViewer.setVisible(true);
  if(index != 1) cheatDatabase.setVisible(false), cheatWindow.setVisible(false);
  if(index != 2) stateWindow.setVisible(false);
  panelContainer.resize();
  setVisible();
  setFocused();
}
