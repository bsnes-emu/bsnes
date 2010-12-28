void add_clocks(unsigned clocks);
void scanline();
void frame();

//opcode.cpp
void op_io();
uint8 op_read(uint16 addr);
void op_write(uint16 addr, uint8 data);
