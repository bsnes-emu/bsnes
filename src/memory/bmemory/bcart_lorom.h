class bCartLoROM : public Cart {
private:
bool   write_protected;
uint32 P0_size, P1_size, ROM_mask;

public:
uint8 *rom, *sram;
uint32 rom_size, sram_size;
  uint8 read (uint32 addr);
  void  write(uint32 addr, uint8 value);
  void  write_protect(bool r);
  void  set_cartinfo(CartInfo *ci);

  bCartLoROM() : write_protected(true) {}
};
