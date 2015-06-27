bool r15_modified = false;

auto step(unsigned clocks) -> void;

auto rombuffer_sync() -> void;
auto rombuffer_update() -> void;
auto rombuffer_read() -> uint8;

auto rambuffer_sync() -> void;
auto rambuffer_read(uint16 addr) -> uint8;
auto rambuffer_write(uint16 addr, uint8 data) -> void;

auto r14_modify(uint16) -> void;
auto r15_modify(uint16) -> void;

auto timing_reset() -> void;
