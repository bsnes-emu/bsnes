void Cartridge::load_rom_normal() {
uint size = 0;
  for(int i = 0; i < file.count; i++) {
    size += file.rom_size[i] - (((file.rom_size[i] & 0x7fff) == 512) ? 512 : 0);
  }

  info.rom_size = size;
  rom = (uint8*)malloc(info.rom_size);
  memset(rom, 0, info.rom_size);

uint offset = 0;
  for(int i = 0; i < file.count; i++) {
  uint8 *data = file.rom_data[i] + (((file.rom_size[i] & 0x7fff) == 512) ? 512 : 0);
  uint   size = file.rom_size[i] - (((file.rom_size[i] & 0x7fff) == 512) ? 512 : 0);
    memcpy(rom + offset, data, size);
    offset += size;
    safe_free(file.rom_data[i]);
  }

  info.crc32 = crc32_calculate(rom, info.rom_size);

  if(read_database() == true) {
    info.header_index = 0xffc0;
    info.mapper = PCB;
    strcpy(info.name, dbi.name);
    strcpy(info.pcb,  dbi.pcb);
    info.region = NTSC;

    info.rom_size = dbi.rom;
    info.ram_size = dbi.ram;
  } else {
    find_header();
    read_header();
  }
}

void Cartridge::load_ram_normal() {
  if(info.ram_size == 0) {
    ram = 0;
    return;
  }

  ram = (uint8*)malloc(info.ram_size);
  memset(ram, 0xff, info.ram_size);

  if(load_file(file.ram_name[0], file.ram_data[0], file.ram_size[0]) == true) {
    memcpy(ram, file.ram_data[0], min(info.ram_size, file.ram_size[0]));
    safe_free(file.ram_data[0]);
  }
}

void Cartridge::save_ram_normal() {
  if(info.ram_size == 0)return;

  save_file(file.ram_name[0], ram, info.ram_size);
}
