/*
  libups : version 0.01 ~byuu, Nach (09/30/06)
*/

#ifndef __LIBUPS
#define __LIBUPS

template<typename FT>
class UPSPatch {
public:

enum {
  FLAG_INFO = 0x00000001,
};

uint32 sig;
uint16 version;
uint32 format;
uint32 flags;
uint64 original_filesize;
uint64 modified_filesize;

uint8  info_author[64 + 8];
uint8  info_version[16 + 8];
uint8  info_title[128 + 8];
uint8  info_genre[32 + 8];
uint8  info_language[32 + 8];
uint8  info_date[8 + 8];
uint8  info_website[256 + 8];
uint8  info_description[1024 + 8];

FT fp;

uint32 xmlsize;
uint32 original_crc32;
uint32 modified_crc32;
uint32 patch_crc32;

  bool load();
  void save_header();
  void save_footer();
  void close();

  UPSPatch();
};

template<typename FTO, typename FTM, typename FTP>
class UPS {
public:

FTO original;
FTM modified;
UPSPatch<FTP> patch;

uint64 original_filesize;
uint64 modified_filesize;
uint64 largest_filesize;

uint32 original_crc32;
uint32 modified_crc32;
uint32 patch_crc32;

bool patch_eof;

  uint ptr_read();
  void ptr_write(uint ptr);

  void create(const char *original_filename,
              const char *modified_filename,
              const char *patch_filename);
  void create_linear();

  bool apply(const char *original_filename,
             const char *modified_filename,
             const char *patch_filename);
  bool apply();
  void apply_linear();
};

/*****
 * UPSPatch
 *****/

template<typename FT>
bool UPSPatch<FT>::load() {
  fseek(fp, 0, file::seek_start);

  patch_crc32 = 0xffffffff;
  while(ftell(fp) < fsize(fp) - 4) {
    patch_crc32 = crc32_adjust(patch_crc32, fgetc(fp));
  }
  patch_crc32 = ~patch_crc32;

  fseek(fp, -16, file::seek_end);

  xmlsize = fgetld(fp);
  original_crc32 = fgetld(fp);
  modified_crc32 = fgetld(fp);

  if(patch_crc32 != fgetld(fp)) {
    fprintf(stdout, "error: patch checksum failure\n");
    return false;
  }

  fseek(fp, 0, file::seek_start);

  sig = fgetld(fp);
  version = fgetlw(fp);
  format = fgetld(fp);
  flags = fgetld(fp);
  original_filesize = fgetlq(fp);
  modified_filesize = fgetlq(fp);

  memset(info_author, 0, 64 + 8);
  memset(info_version, 0, 16 + 8);
  memset(info_title, 0, 128 + 8);
  memset(info_genre, 0, 32 + 8);
  memset(info_language, 0, 32 + 8);
  memset(info_date, 0, 8 + 8);
  memset(info_website, 0, 256 + 8);
  memset(info_description, 0, 1024 + 8);

  if(flags & FLAG_INFO) {
    fread(fp, info_author, 64);
    fread(fp, info_version, 16);
    fread(fp, info_title, 128);
    fread(fp, info_genre, 32);
    fread(fp, info_language, 32);
    fread(fp, info_date, 8);
    fread(fp, info_website, 256);
    fread(fp, info_description, 1024);
  }

  return true;
}

template<typename FT>
void UPSPatch<FT>::save_header() {
  fputld(fp, sig);
  fputlw(fp, version);
  fputld(fp, format);
  fputld(fp, flags);
  fputlq(fp, original_filesize);
  fputlq(fp, modified_filesize);

  if(flags & FLAG_INFO) {
    fwrite(fp, info_author, 64);
    fwrite(fp, info_version, 16);
    fwrite(fp, info_title, 128);
    fwrite(fp, info_genre, 32);
    fwrite(fp, info_language, 32);
    fwrite(fp, info_date, 8);
    fwrite(fp, info_website, 256);
    fwrite(fp, info_description, 1024);
  }
}

template<typename FT>
void UPSPatch<FT>::save_footer() {
  fputld(fp, xmlsize);
  fputld(fp, original_crc32);
  fputld(fp, modified_crc32);

  patch_crc32 = fcrc32(fp);
  fputld(fp, patch_crc32);
}

template<typename FT>
void UPSPatch<FT>::close() {
  fclose(fp);
}

template<typename FT>
UPSPatch<FT>::UPSPatch() {
  sig = 0x00737075; //'ups\0'
  version = 0x0001; //1.0
  format = 0x00000000; //linear
  flags = 0;
  original_filesize = 0;
  modified_filesize = 0;

  memset(info_author, 0, 64 + 8);
  memset(info_version, 0, 16 + 8);
  memset(info_title, 0, 128 + 8);
  memset(info_genre, 0, 32 + 8);
  memset(info_language, 0, 32 + 8);
  memset(info_date, 0, 8 + 8);
  memset(info_website, 0, 256 + 8);
  memset(info_description, 0, 1024 + 8);

  xmlsize = 0;
  original_crc32 = 0;
  modified_crc32 = 0;
  patch_crc32 = 0;
}

/*****
 * UPS
 *****/

template<typename FTO, typename FTM, typename FTP>
uint UPS<FTO, FTM, FTP>::ptr_read() {
uint len = fgetc(patch.fp);
  if(len == 0xff) {
    patch_eof = true;
    return 0;
  }

  if(len <= 0xf7) {
    return len;
  }

  len -= 0xf8;
uint ptr = 0;
  for(int i = 0; i < (len + 2); i++) {
    ptr |= fgetc(patch.fp) << (i << 3);
  }

  ptr += 0xf8;
  if(len >= 1) { ptr += 0x10000; }
  if(len >= 2) { ptr += 0x1000000; }

  return ptr;
}

template<typename FTO, typename FTM, typename FTP>
void UPS<FTO, FTM, FTP>::ptr_write(uint ptr) {
  if(ptr <= 0xf7) {
    fputc(patch.fp, ptr);
    return;
  }

  ptr -= 0xf8;
  if(ptr <= 0xffff) {
    fputc(patch.fp, 0xf8);
    fputc(patch.fp, ptr);
    fputc(patch.fp, ptr >> 8);
    return;
  }

  ptr -= 0x10000;
  if(ptr <= 0xffffff) {
    fputc(patch.fp, 0xf9);
    fputc(patch.fp, ptr);
    fputc(patch.fp, ptr >> 8);
    fputc(patch.fp, ptr >> 16);
    return;
  }

  ptr -= 0x1000000;
  fputc(patch.fp, 0xfa);
  fputc(patch.fp, ptr);
  fputc(patch.fp, ptr >> 8);
  fputc(patch.fp, ptr >> 16);
  fputc(patch.fp, ptr >> 24);
}

template<typename FTO, typename FTM, typename FTP>
void UPS<FTO, FTM, FTP>::create(
const char *original_filename,
const char *modified_filename,
const char *patch_filename
) {
  fopen(original, original_filename, file::mode_read);
  fopen(modified, modified_filename, file::mode_read);
  fopen(patch.fp, patch_filename, file::mode_writeread);

  patch.original_filesize = original_filesize = fsize(original);
  patch.modified_filesize = modified_filesize = fsize(modified);
  largest_filesize = (original_filesize >= modified_filesize) ?
                     original_filesize : modified_filesize;

  patch.save_header();

  switch(patch.format) {
  case 0x00000000: create_linear(); break;
  }

  patch.original_crc32 = original_crc32 = fcrc32(original);
  patch.modified_crc32 = modified_crc32 = fcrc32(modified);

  patch.save_footer();
  patch.close();
}

template<typename FTO, typename FTM, typename FTP>
void UPS<FTO, FTM, FTP>::create_linear() {
  fseek(original, 0, file::seek_start);
  fseek(modified, 0, file::seek_start);

uint last_ptr = 0, rle_count, last_out, rep_count;
  for(int i = 0; i < largest_filesize;) {
  uint8 r = fgetc(original) ^ fgetc(modified);
    i++;
    if(r == 0x00)continue;

  //ptr
    ptr_write((i - 1) - last_ptr);

  //data
    fputc(patch.fp, r);
    last_out  = r;
    rep_count = 0;

    do {
      r = fgetc(original) ^ fgetc(modified);
      i++;
      fputc(patch.fp, r);

      if(last_out != r) {
        rep_count = 0;
      } else {
        if(++rep_count == (3 - 1)) {
          rle_count = 0;
          do {
            r = fgetc(original) ^ fgetc(modified);
            if(r != last_out || r == 0x00) { break; }
            rle_count++;
          } while(i < largest_filesize);

          ptr_write(rle_count);
          if(i < largest_filesize) { fputc(patch.fp, r); }
          rep_count = 0;
        }
      }

      last_out = r;
      if(r == 0x00) { break; }
    } while(i < largest_filesize);

    last_ptr = i;
  }

  fputc(patch.fp, 0xff);
}

template<typename FTO, typename FTM, typename FTP>
bool UPS<FTO, FTM, FTP>::apply(
const char *original_filename,
const char *modified_filename,
const char *patch_filename
) {
  fopen(original, original_filename, file::mode_read);
  fopen(modified, modified_filename, file::mode_writeread);
  fopen(patch.fp, patch_filename, file::mode_read);

  return apply();
}

template<typename FTO, typename FTM, typename FTP>
bool UPS<FTO, FTM, FTP>::apply() {
  patch.load();
  patch_eof = false;

  original_filesize = fsize(original);
  original_crc32    = fcrc32(original);

  if(original_filesize == patch.original_filesize &&
     original_crc32 == patch.original_crc32) {
    modified_filesize = patch.modified_filesize;
    modified_crc32    = patch.modified_crc32;
  } else if(original_filesize == patch.modified_filesize &&
            original_crc32 == patch.modified_crc32) {
    modified_filesize = patch.original_filesize;
    modified_crc32    = patch.original_crc32;
  } else {
    fprintf(stdout, "error: input checksum failure\n");
    return false;
  }

  patch_crc32 = patch.patch_crc32;

  apply_linear();
  patch.close();

  if(modified_crc32 != fcrc32(modified)) {
    fprintf(stdout, "error: output checksum failure\n");
  }

  return true;
}

template<typename FTO, typename FTM, typename FTP>
void UPS<FTO, FTM, FTP>::apply_linear() {
  fseek(original, 0, file::seek_start);
  fseek(modified, 0, file::seek_start);
  for(uint i = 0; i < modified_filesize; i++) {
    fputc(modified, fgetc(original));
  }

  fseek(original, 0, file::seek_start);
  fseek(modified, 0, file::seek_start);

uint rle_count, last_in, rep_count;
  while(!feof(patch.fp) && patch_eof == false && ftell(modified) < modified_filesize) {
  //ptr
  uint ptr = ptr_read();
    if(patch_eof == true) { break; }

  //data
    fseek(original, ftell(original) + ptr, file::seek_start);
    fseek(modified, ftell(modified) + ptr, file::seek_start);

    last_in   = 0;
    rep_count = 0;

    do {
      if(ftell(modified) >= modified_filesize) { break; }

    uint8 r = fgetc(patch.fp);
      fputc(modified, r ^ fgetc(original));
      if(r != last_in) {
        rep_count = 0;
      } else {
        if(++rep_count == (3 - 1)) {
          rle_count = ptr_read();
          while(rle_count-- && ftell(modified) < modified_filesize) {
            fputc(modified, r ^ fgetc(original));
          }
          rep_count = 0;
        }
      }

      last_in = r;
      if(r == 0x00) { break; }
    } while(!feof(patch.fp) && patch_eof == false && ftell(modified) < modified_filesize);
  }
}

#endif
