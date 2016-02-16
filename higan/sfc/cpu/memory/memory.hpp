auto op_io() -> void override;
auto op_read(uint24 addr) -> uint8 override;
auto op_write(uint24 addr, uint8 data) -> void override;
alwaysinline auto speed(uint24 addr) const -> uint;

auto disassembler_read(uint24 addr) -> uint8 override;
