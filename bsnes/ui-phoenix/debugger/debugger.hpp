#include "console.hpp"
#include "cpu/debugger.hpp"

struct Debugger : TopLevelWindow {
  enum class DebugMode : unsigned {
    None,
    StepIntoCPU,
  } debugMode;

  CheckBox enableDebugger;
  CheckBox showMemoryEditor;

  void create();
  void setVisible(bool visible = true);
  void enable(bool state);
  void run();

  Debugger();
};

extern Debugger debugger;
