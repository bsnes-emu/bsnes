void add_clocks(unsigned clocks);
void timer_stage0();
void timer_stage1();
void timer_stage2();
void timer_stage3();

//opcode.cpp
void op_io();
uint8 op_read(uint16 addr);
void op_write(uint16 addr, uint8 data);
