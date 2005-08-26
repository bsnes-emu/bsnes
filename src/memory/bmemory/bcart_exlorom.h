class bCartExLoROM : public Cart {
private:
bool write_protected;

public:
uint8 *rom, *sram;
uint32 rom_size, sram_size;
  uint8 read (uint32 addr);
  void  write(uint32 addr, byte value);
  void  write_protect(bool r);
  void  set_cartinfo(CartInfo *ci);

  bCartExLoROM() : write_protected(true) {}
};
