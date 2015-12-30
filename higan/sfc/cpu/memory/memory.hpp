auto op_io() -> void;
auto op_read(uint32 addr) -> uint8;
auto op_write(uint32 addr, uint8 data) -> void;
alwaysinline auto speed(uint addr) const -> uint;

auto disassembler_read(uint32 addr) -> uint8;
