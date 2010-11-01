#include "console.hpp"
#include "cpu/debugger.hpp"
#include "smp/debugger.hpp"
#include "tools/breakpoint-editor.hpp"
#include "tools/memory-editor.hpp"

struct Debugger : TopLevelWindow {
  enum class DebugMode : unsigned {
    None,
    WaitForBreakpoint,
    StepIntoCPU,
    StepIntoSMP,
  } debugMode;

  CheckBox enableDebugger;
  CheckBox showConsole;
  CheckBox showCPUDebugger;
  CheckBox showSMPDebugger;
  CheckBox showBreakpointEditor;
  CheckBox showMemoryEditor;

  void create();
  void setVisible(bool visible = true);
  void enable(bool state);
  void run();

  Debugger();
};

extern Debugger debugger;
