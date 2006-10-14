bool TracerSettings::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    Hide();
    return true;
  } break;

  case EVENT_CLICKED: {
    if(info.control == &TracerEnable) {
      tracer.enable(!tracer.enabled());
      TracerEnable.SetText("%s", tracer.enabled() ? "Disable Tracer" : "Enable Tracer");
    } else if(info.control == &CPUTraceOp) {
      tracer.cpuop_enable(CPUTraceOp.Checked());
    } else if(info.control == &CPUTraceOpMask) {
      tracer.cpuopmask_enable(CPUTraceOpMask.Checked());
    } else if(info.control == &CPUTraceDMA) {
      tracer.cpudma_enable(CPUTraceDMA.Checked());
    } else if(info.control == &SMPTraceOp) {
      tracer.smpop_enable(SMPTraceOp.Checked());
    } else if(info.control == &SMPTraceOpMask) {
      tracer.smpopmask_enable(SMPTraceOpMask.Checked());
    }
  } break;

  }

  return false;
}

void TracerSettings::Setup() {
int x = 5, y = 5;
  TracerEnable.Create(this, "visible", x, y, 120, 25);
  TracerEnable.SetText("%s", tracer.enabled() ? "Disable Tracer" : "Enable Tracer");
  y += 30;

  CPUGroup.Create(this, "visible", x, y, 160, 120, "S-CPU");
  CPUTraceOp.Create(this, "visible|auto", x + 5, y + 15, 150, 15, "Opcodes");
  CPUTraceOpMask.Create(this, "visible|auto", x + 5, y + 30, 150, 15, "Opcode mask");
  CPUTraceDMA.Create(this, "visible|auto", x + 5, y + 45, 150, 15, "DMA");
  CPUTraceHDMA.Create(this, "visible|auto|disabled", x + 5, y + 60, 150, 15, "HDMA");
  x += 165;

  SMPGroup.Create(this, "visible", x, y, 160, 120, "S-SMP");
  SMPTraceOp.Create(this, "visible|auto", x + 5, y + 15, 150, 15, "Opcodes");
  SMPTraceOpMask.Create(this, "visible|auto", x + 5, y + 30, 150, 15, "Opcode mask");
  x += 165;
}
