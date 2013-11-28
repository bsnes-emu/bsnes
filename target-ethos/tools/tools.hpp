struct ToolsLayout : VerticalLayout {
  ToolsLayout();
};

#include "cheat-database.hpp"
#include "cheat-editor.hpp"
#include "state-manager.hpp"

struct Tools : Window {
  VerticalLayout layout;
  TabFrame panels;

  Tools();
};

extern Tools* tools;
