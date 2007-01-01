void Cartridge::load_rom_stdual() {
uint8 *data;
uint   size;
string bios = config::file_updatepath("stbios.bin", config::path.bios);
  info.rom_size = 0x300000;
  rom = (uint8*)malloc(info.rom_size);
  memset(rom, 0, info.rom_size);

  load_file(strptr(bios), data, size);
  memcpy(rom, data, min(size, 0x040000));
  safe_free(data);

  memcpy(rom + 0x100000, file.rom_data[0], min(file.rom_size[0], 0x100000));
  safe_free(file.rom_data[0]);

  memcpy(rom + 0x200000, file.rom_data[1], min(file.rom_size[1], 0x100000));
  safe_free(file.rom_data[1]);

char name_a[4096], name_b[4096];
  strcpy(name_a, "???");
  strcpy(name_b, "???");
//
  info.mapper = PCB;
  info.region = NTSC;

  info.rom_size = 0x300000;
  info.ram_size = 0x040000;
//

  info.crc32 = crc32_calculate(rom + 0x100000, file.rom_size[0]);
  if(read_database() == true) {
    strcpy(name_a, dbi.name);
  }

  info.crc32 = crc32_calculate(rom + 0x200000, file.rom_size[1]);
  if(read_database() == true) {
    strcpy(name_b, dbi.name);
  }

//
  info.crc32 = 0;
  strcpy(info.name, name_a);
  strcat(info.name, " + ");
  strcat(info.name, name_b);
  strcpy(info.pcb, "STC-DUAL");
}

void Cartridge::load_ram_stdual() {
  ram = (uint8*)malloc(info.ram_size);
  memset(ram, 0xff, info.ram_size);

  if(load_file(file.ram_name[0], file.ram_data[0], file.ram_size[0]) == true) {
    memcpy(ram + 0x000000, file.ram_data[0], min(file.ram_size[0], 0x020000));
    safe_free(file.ram_data[0]);
  }

  if(load_file(file.ram_name[1], file.ram_data[1], file.ram_size[1]) == true) {
    memcpy(ram + 0x020000, file.ram_data[1], min(file.ram_size[1], 0x020000));
    safe_free(file.ram_data[1]);
  }
}

void Cartridge::save_ram_stdual() {
  save_file(file.ram_name[0], ram + 0x000000, 0x020000);
  save_file(file.ram_name[1], ram + 0x020000, 0x020000);
}
