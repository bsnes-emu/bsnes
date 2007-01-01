void Cartridge::load_rom_st() {
uint8 *data;
uint   size;
string bios = config::file_updatepath("stbios.bin", config::path.bios);
  info.rom_size = 0x200000;
  rom = (uint8*)malloc(info.rom_size);
  memset(rom, 0, info.rom_size);

  load_file(strptr(bios), data, size);
  memcpy(rom, data, min(size, 0x040000));
  safe_free(data);

  memcpy(rom + 0x100000, file.rom_data[0], min(file.rom_size[0], 0x100000));
  safe_free(file.rom_data[0]);

//
  strcpy(info.name, "???");
  strcpy(info.pcb,  "STC-SOLO");

  info.mapper = PCB;
  info.region = NTSC;

  info.rom_size = 0x200000;
  info.ram_size = 0x020000;
//

  info.crc32 = crc32_calculate(rom + 0x100000, file.rom_size[0]);
  if(read_database() == true) {
    strcpy(info.name, dbi.name);
  }
}

void Cartridge::load_ram_st() {
  ram = (uint8*)malloc(info.ram_size);
  memset(ram, 0xff, info.ram_size);

  if(load_file(file.ram_name[0], file.ram_data[0], file.ram_size[0]) == true) {
    memcpy(ram, file.ram_data[0], min(file.ram_size[0], 0x020000));
    safe_free(file.ram_data[0]);
  }
}

void Cartridge::save_ram_st() {
  save_file(file.ram_name[0], ram, 0x020000);
}
