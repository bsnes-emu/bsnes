struct db_item {
  uint32 crc32;
  char   name[128];
  char   pcb [32];
  uint32 rom;
  uint32 ram;
};

void db_write(FILE *fp, db_item &dbi) {
  fputc(dbi.crc32 >>  0, fp);
  fputc(dbi.crc32 >>  8, fp);
  fputc(dbi.crc32 >> 16, fp);
  fputc(dbi.crc32 >> 24, fp);

  fwrite(dbi.name, 1, 128, fp);
  fwrite(dbi.pcb,  1,  32, fp);

  fputc(dbi.rom >>  0, fp);
  fputc(dbi.rom >>  8, fp);
  fputc(dbi.rom >> 16, fp);
  fputc(dbi.rom >> 24, fp);

  fputc(dbi.ram >>  0, fp);
  fputc(dbi.ram >>  8, fp);
  fputc(dbi.ram >> 16, fp);
  fputc(dbi.ram >> 24, fp);
}

void db_read(db_item &dbi, uint8 *data) {
  dbi.crc32  = (*data++) <<  0;
  dbi.crc32 |= (*data++) <<  8;
  dbi.crc32 |= (*data++) << 16;
  dbi.crc32 |= (*data++) << 24;

  memcpy(dbi.name, data, 128); dbi.name[127] = 0; data += 128;
  memcpy(dbi.pcb,  data,  32); dbi.pcb [ 31] = 0; data +=  32;

  dbi.rom  = (*data++) <<  0;
  dbi.rom |= (*data++) <<  8;
  dbi.rom |= (*data++) << 16;
  dbi.rom |= (*data++) << 24;

  dbi.ram  = (*data++) <<  0;
  dbi.ram |= (*data++) <<  8;
  dbi.ram |= (*data++) << 16;
  dbi.ram |= (*data++) << 24;
}
