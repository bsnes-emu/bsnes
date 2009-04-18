void op_io();
uint8_t op_read(unsigned addr);
void op_write(unsigned addr, uint8_t data);
void cycle_edge();
unsigned bus_speed(unsigned addr);
