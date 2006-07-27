void Cartridge::load_database() {
  database      = 0;
  database_size = 0;

FILE *fp;
  fp = fopen("cart.db", "rb");
  if(!fp)return;

uint size = fsize(fp);
  if(size < 8) {
    fclose(fp);
    return;
  }

  database = (uint8*)malloc(size);
  fread(database, 1, size, fp);
  fclose(fp);

  database_blocksize = (database[6] << 0) | (database[7] << 8);
  database_size      = (size - 8) / database_blocksize;
}

bool Cartridge::read_database() {
uint i, crc32;
  for(i = 0; i < database_size; i++) {
  uint8 *p = database + 8 + (i * database_blocksize);
    crc32  = *(p++) <<  0;
    crc32 |= *(p++) <<  8;
    crc32 |= *(p++) << 16;
    crc32 |= *(p++) << 24;
    if(crc32 == cartridge.info.crc32)break;
  }
  if(i >= database_size)return false;

  db_read(dbi, database + 8 + (i * database_blocksize));
  return true;
}
