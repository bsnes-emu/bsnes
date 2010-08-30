class CPUDebugger : public CPU, public ChipDebugger {
public:
  bool property(unsigned id, string &name, string &value);

  function<void ()> step_event;

  enum Usage {
    UsageRead  = 0x80,
    UsageWrite = 0x40,
    UsageExec  = 0x20,
    UsageFlagM = 0x02,
    UsageFlagX = 0x01,
  };
  uint8 *usage;
  uint32 opcode_pc;  //points to the current opcode, used to backtrace on read/write breakpoints

  void op_step();
  uint8 op_read(uint32 addr);
  void op_write(uint32 addr, uint8 data);

  CPUDebugger();
  ~CPUDebugger();
};
