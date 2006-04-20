/*
  libbpf : version 0.01 ~byuu (12/18/05)
*/

#ifndef __LIBBPF
#define __LIBBPF

#include "libbase.h"
#include "libvector.h"

class BPF;

class BPF {
public:

enum {
//patch formats
  FORMAT_BINARY = 0x0000,
  FORMAT_SNES   = 0x0001,
};

enum {
  INDEX_SIG      = 0x00, //4 bytes
  INDEX_VERSION  = 0x04, //2 bytes
  INDEX_PTRSIZE  = 0x06, //2 bytes
  INDEX_FORMAT   = 0x08, //4 bytes
  INDEX_FLAGS    = 0x0c, //4 bytes
  INDEX_VARIABLE = 0x10, //variable data follows
};

enum { MODE_MEMORY, MODE_FILE };
enum { ACCESS_READ, ACCESS_WRITE };

struct Handle {
  BPF   *parent;
  uint8  mode, access;
  vector<uint8> data;
  FILE  *handle;
  uint32 size, pos, crc32;

  void   seek(uint32 new_pos);
  void   write(uint8 x);
  uint8  read();

  uint32 calc_crc32();

  bool   open(uint8 _mode, uint32 size, uint8 *_data = 0);
  bool   open(uint8 _mode, const char *fn);
  bool   load(const char *fn);
  bool   save(const char *fn);
  void   close();

  void   set_parent(BPF *_bpf) { parent = _bpf; }

  Handle() {
    mode  = 0; access = 0;
    crc32 = 0; handle = 0;
    size  = 0; pos    = 0;
  }
} original, modified, output, input, patch;

struct {
  uint32 memory_limit;
  struct {
    bool   enabled;
    uint8  mode;
    char   fn[4096];
    FILE  *handle;
    uint8 *data;
    uint32 size;
  } insert_data;
} settings;

struct {
  uint16 ptr_size;
  uint32 size_min, size_max;
  uint32 patch_start;
  uint32 format;
} info;

  uint8 *load(const char *fn, uint32 &size);
  void   save(const char *fn, uint8 *data, uint32 size);

  void   write_ptr(uint32 ptr);
  uint32 read_ptr();
  void   create_patch_binary();
  bool   create_patch(uint32 format, const char *fn);
  bool   create_patch(uint32 format, const char *fn_patch, const char *fn_x, const char *fn_y);

  void   apply_patch_binary();
  bool   apply_patch();
  bool   apply_patch(const char *fn_patch, const char *fn_input);
  bool   apply_patch(uint32 s_patch, uint8 *p_patch, uint32 s_input, uint8 *p_input);

  uint8 *get_output_handle(uint32 &size);
  void   save_output(const char *fn);

  void   clear_settings();

  BPF();
  ~BPF();
};

#endif
