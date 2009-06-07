uint8_t op_read(uint16_t addr);
alwaysinline uint8_t peekpipe();
alwaysinline uint8_t pipe();

void cache_flush();
uint8_t cache_mmio_read(uint16_t addr);
void cache_mmio_write(uint16_t addr, uint8_t data);

void memory_reset();
