unsigned cache_access_speed;
unsigned memory_access_speed;
bool r15_modified;

void add_clocks(unsigned clocks);

void rombuffer_sync();
void rombuffer_update();
uint8_t rombuffer_read();

void rambuffer_sync();
uint8_t rambuffer_read(uint16_t addr);
void rambuffer_write(uint16_t addr, uint8_t data);

void r14_modify(uint16_t);
void r15_modify(uint16_t);

void timing_reset();
