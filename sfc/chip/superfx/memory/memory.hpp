unsigned rom_mask;  //rom_size - 1
unsigned ram_mask;  //ram_size - 1

auto bus_read(unsigned addr) -> uint8;
auto bus_write(unsigned addr, uint8 data) -> void;

auto op_read(uint16 addr) -> uint8;
alwaysinline auto peekpipe() -> uint8;
alwaysinline auto pipe() -> uint8;

auto cache_flush() -> void;
auto cache_mmio_read(uint16 addr) -> uint8;
auto cache_mmio_write(uint16 addr, uint8 data) -> void;

auto memory_reset() -> void;
