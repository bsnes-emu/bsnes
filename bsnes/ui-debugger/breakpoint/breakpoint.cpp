#include "../base.hpp"
BreakpointEditor *breakpointEditor = nullptr;

BreakpointEntry::BreakpointEntry() {
  enable.setText("Enable");
  addr.setFont(application->monospaceFont);
  data.setFont(application->monospaceFont);
  type.append("Read", "Write", "Exec");
  source.append("CPU-Bus", "APU-Bus", "VRAM", "OAM", "CGRAM");

  append(enable, {0, 0}, 5);
  append(addr, {50, 0}, 5);
  append(data, {25, 0}, 5);
  append(type, {0, 0}, 5);
  append(source, {0, 0});
}

BreakpointEditor::BreakpointEditor() {
  setTitle("Breakpoint Editor");

  layout.setMargin(5);
  for(auto &bp : breakpoint) layout.append(bp, {0, 0}, 5);
  append(layout);

  setGeometry({800, 600, layout.minimumGeometry().width, layout.minimumGeometry().height - 5});
}
