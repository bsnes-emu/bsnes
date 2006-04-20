#include "libbase.h"
#include "libvector.h"
#include "libbpf.h"

void BPF::Handle::seek(uint32 new_pos) {
  if(pos == new_pos)return;
  pos = new_pos;
  if(mode == MODE_FILE) {
    fseek(handle, new_pos, SEEK_SET);
  }
}

void BPF::Handle::write(uint8 x) {
  if(mode == MODE_MEMORY) {
    data[pos] = x;
  } else {
    fputc(x, handle);
  }

  if(++pos > size)size = pos;
  crc32 = crc32_adjust(crc32, x);
}

uint8 BPF::Handle::read() {
uint8 r;
  if(pos >= size) {
    return 0x00;
  } else if(mode == MODE_MEMORY) {
    r = data[pos];
  } else {
    r = fgetc(handle);
  }

  pos++;
  crc32 = crc32_adjust(crc32, r);
  return r;
}

uint32 BPF::Handle::calc_crc32() {
  crc32 = 0xffffffff;
  seek(0);
  for(uint32 i = 0; i < size; i++) {
    read();
  }
  seek(0);
  crc32 = ~crc32;
  return crc32;
}

bool BPF::Handle::open(uint8 _access, uint32 _size, uint8 *_data) {
  close();

  mode   = MODE_MEMORY;
  access = _access;
  pos    = 0;
  size   = _size;
  crc32  = 0xffffffff;

  if(access == ACCESS_READ) {
    data.resize(_size);
    data.copy(_data, _size);
  } else {
    data.resize(_size);
    data.clear();
  }

  return true;
}

bool BPF::Handle::open(uint8 _access, const char *fn) {
  close();

  mode   = MODE_FILE;
  access = _access;
  pos    = 0;
  crc32  = 0xffffffff;

  if(access == ACCESS_READ) {
    handle = fopen(fn, "rb");
    if(!handle)return false;
    size = fsize(handle);
  } else if(access == ACCESS_WRITE) {
    handle = fopen(fn, "wb");
    if(!handle)return false;
    size = 0;
  }

  return true;
}

bool BPF::Handle::load(const char *fn) {
  close();

FILE *fp = fopen(fn, "rb");
  if(!fp)return false;
  size = fsize(fp);
uint8 *buffer = (uint8*)malloc(size);
  memset(buffer, 0, size);
  fread(buffer, 1, size, fp);
  fclose(fp);
  return open(ACCESS_READ, size, buffer);
}

bool BPF::Handle::save(const char *fn) {
  if(mode != MODE_MEMORY)return false;

FILE *fp = fopen(fn, "wb");
  if(!fp)return false;
  fwrite(data.handle(), 1, size, fp);
  fclose(fp);
  return true;
}

void BPF::Handle::close() {
  if(mode == MODE_MEMORY) {
    data.release();
  } else if(mode == MODE_FILE) {
    if(handle) {
      fclose(handle);
      handle = 0;
    }
  }
}

uint8 *BPF::load(const char *fn, uint32 &size) {
FILE *fp = fopen(fn, "rb");
  if(!fp)return 0;
  size = fsize(fp);
uint8 *buffer = (uint8*)malloc(size);
  fread(buffer, 1, size, fp);
  fclose(fp);
  fp = 0;
  return buffer;
}

void BPF::save(const char *fn, uint8 *data, uint32 size) {
FILE *fp = fopen(fn, "wb");
  if(!fp)return;
  fwrite(data, 1, size, fp);
  fclose(fp);
  fp = 0;
}

void BPF::write_ptr(uint32 ptr) {
  if(ptr <= 0xef) {
    patch.write(ptr);
    return;
  }

  ptr -= 0xf0;
  if(ptr <= 0xffff) {
    patch.write(0xf0);
    patch.write(ptr);
    patch.write(ptr >> 8);
    return;
  }

  ptr -= 0x10000;
  if(ptr <= 0xffffff) {
    patch.write(0xf1);
    patch.write(ptr);
    patch.write(ptr >> 8);
    patch.write(ptr >> 16);
    return;
  }

  ptr -= 0x1000000;
  patch.write(0xf2);
  patch.write(ptr);
  patch.write(ptr >> 8);
  patch.write(ptr >> 16);
  patch.write(ptr >> 24);
}

uint32 BPF::read_ptr() {
uint32 len = patch.read();
  if(len <= 0xef) {
    return len;
  }

  len &= 0x0f;
uint32 ptr = 0;
  for(int i = 0; i < (len + 2); i++) {
    ptr |= patch.read() << (i << 3);
  }

  ptr += 0xf0;
  if(len >= 1)ptr += 0x10000;
  if(len >= 2)ptr += 0x1000000;

  return ptr;
}

void BPF::create_patch_binary() {
  original.seek(0);
  modified.seek(0);

uint32 last_ptr = 0, rle_count, last_out, rep_count;
  for(uint32 i = 0; i < info.size_max;) {
  uint8 r = original.read() ^ modified.read();
    i++;
    if(r == 0x00)continue;

  //ptr
    write_ptr((i - 1) - last_ptr);

  //data
    patch.write(r);
    last_out  = r;
    rep_count = 0;
    do {
      r = original.read() ^ modified.read();
      i++;
      patch.write(r);
      if(last_out == r) {
        if(++rep_count == 2) {
          rle_count = 0;
          do {
            r = original.read() ^ modified.read();
            i++;
            if(r != last_out || r == 0x00)break;
            rle_count++;
          } while(i < info.size_max);
          write_ptr(rle_count);
          if(i < info.size_max)patch.write(r);
          rep_count = 0;
        }
      } else {
        rep_count = 0;
      }

      last_out = r;
      if(r == 0x00)break;
    } while(i < info.size_max);

    last_ptr = i;
  }
}

bool BPF::create_patch(uint32 format, const char *fn) {
  patch.open(ACCESS_WRITE, fn);

//header
  patch.write('b');
  patch.write('p');
  patch.write('f');
  patch.write(0x00);

//version (high-byte = major, low-byte = minor)
  patch.write(0x00);
  patch.write(0x01);

//bytes per pointer
  info.size_max = (original.size >= modified.size) ? original.size : modified.size;
  info.size_min = (original.size >= modified.size) ? modified.size : original.size;

  if(info.size_max <= 0xff) {
    info.ptr_size = 1;
  } else if(info.size_max <= 0xffff) {
    info.ptr_size = 2;
  } else if(info.size_max <= 0xffffff) {
    info.ptr_size = 3;
  } else {
    info.ptr_size = 4;
  }
  patch.write(info.ptr_size);
  patch.write(info.ptr_size >> 8);

//format
  patch.write(format);
  patch.write(format >> 8);
  patch.write(format >> 16);
  patch.write(format >> 24);

//flags
uint32 flags = 0;
  patch.write(flags);
  patch.write(flags >> 8);
  patch.write(flags >> 16);
  patch.write(flags >> 24);

//original size
  for(int i = 0; i < info.ptr_size; i++) {
    patch.write(original.size >> (i << 3));
  }

//modified size
  for(int i = 0; i < info.ptr_size; i++) {
    patch.write(modified.size >> (i << 3));
  }

//patch data offset (currently unused)
uint32 pos = patch.pos + 4;
  for(int i = 0; i < 4; i++) {
    patch.write(pos >> (i << 3));
  }

  create_patch_binary();

//write crc32 of original file
  for(int i = 0; i < 4; i++) {
    patch.write(~original.crc32 >> (i << 3));
  }

//write crc32 of modified file
  for(int i = 0; i < 4; i++) {
    patch.write(~modified.crc32 >> (i << 3));
  }

uint32 patch_crc32 = patch.crc32;
//write crc32 of patch file
  for(int i = 0; i < 4; i++) {
    patch.write(~patch_crc32 >> (i << 3));
  }

  patch.close();
  return true;
}

void BPF::apply_patch_binary() {
//copy old data info output buffer
  output.seek(0);
  input.seek(0);
  for(uint32 z = 0; z < output.size; z++) {
    output.write(input.read());
  }

  output.seek(0);
  input.seek(0);
  patch.seek(info.patch_start);
//the below routine may modify output.size if the input
//size is larger, so save the correct output size...
uint32 start_size = output.size;

//subtract 12 to ignore crc32 for original, modified, and patch files
uint32 rle_count, last_in, rep_count;
  for(; patch.pos < (patch.size - 12);) {
  //ptr
  uint32 ptr = read_ptr();

  //data
    output.seek(output.pos + ptr);
    input.seek(input.pos + ptr);

    last_in   = 0;
    rep_count = 0;
    do {
    uint8 r = patch.read();
      output.write(r ^ input.read());
      if(r == last_in) {
        if(++rep_count == 2) {
          rle_count = read_ptr();
          while(rle_count--) {
            output.write(r ^ input.read());
          }
          rep_count = 0;
        }
      } else {
        rep_count = 0;
      }

      last_in = r;
      if(r == 0x00)break;
    } while(patch.pos < (patch.size - 12));
  }

//...and restore it when finished patching
  output.size = start_size;
}

bool BPF::apply_patch() {
//verify patch is large enough to be a valid patchfile
  if(patch.size < 34)return false;

  patch.seek(INDEX_SIG);
//verify signature
  if(patch.read() != 'b')return false;
  if(patch.read() != 'p')return false;
  if(patch.read() != 'f')return false;
  if(patch.read() != 0x00)return false;

//read pointer size
  patch.seek(INDEX_PTRSIZE);
  info.ptr_size  = patch.read();
  info.ptr_size |= patch.read() << 8;

//read flags
uint32 flags;
  patch.seek(INDEX_FLAGS);
  flags  = patch.read();
  flags |= patch.read() << 8;
  flags |= patch.read() << 16;
  flags |= patch.read() << 24;

uint32 sx = 0, sy = 0;
  patch.seek(INDEX_VARIABLE);
  for(int i = 0; i < info.ptr_size; i++) {
    sx |= patch.read() << (i << 3);
  }
  for(int i = 0; i < info.ptr_size; i++) {
    sy |= patch.read() << (i << 3);
  }

uint32 cx = 0, cy = 0, cp = 0;
  patch.seek(patch.size - 12);
  for(int i = 0; i < 4; i++) {
    cx |= patch.read() << (i << 3);
  }
  for(int i = 0; i < 4; i++) {
    cy |= patch.read() << (i << 3);
  }
  for(int i = 0; i < 4; i++) {
    cp |= patch.read() << (i << 3);
  }

  input.calc_crc32();
//skip the stored patch crc32 when calculating
  patch.size -= 4;
  patch.calc_crc32();
  patch.size += 4;

//validate patch crc32
  if(patch.crc32 != cp) {
    return false;
  }

//validate input crc32 + size
bool input_num = 0;
  if(input.crc32 == cx && input.size == sx) {
    output.open(ACCESS_WRITE, sy);
    input_num = 0;
  } else if(input.crc32 == cy && input.size == sy) {
    output.open(ACCESS_WRITE, sx);
    input_num = 1;
  } else {
    return false;
  }

  patch.seek(INDEX_FORMAT);
  info.format = 0;
  for(int i = 0; i < 4; i++) {
    info.format |= patch.read() << (i << 3);
  }

  info.patch_start = 0;
  patch.seek(INDEX_VARIABLE + info.ptr_size * 2);
  for(int i = 0; i < 4; i++) {
    info.patch_start |= patch.read() << (i << 3);
  }

  apply_patch_binary();

  output.calc_crc32();
  return (output.crc32 == ((input_num == 0) ? cy : cx));
}

//main library interface functions

bool BPF::create_patch(uint32 format, const char *fn_patch, const char *fn_x, const char *fn_y) {
uint32 size_x = fsize(fn_x);
uint32 size_y = fsize(fn_y);
bool   lim_x  = (size_x >= settings.memory_limit);
bool   lim_y  = (size_y >= settings.memory_limit);
  switch(format) {
  case FORMAT_BINARY: {
    if(lim_x == false) {
      original.load(fn_x);
    } else {
      original.open(ACCESS_READ, fn_x);
    }
    if(lim_y == false) {
      modified.load(fn_y);
    } else {
      modified.open(ACCESS_READ, fn_y);
    }
  } break;
  case FORMAT_SNES: {
    if(lim_x == true || lim_y == true) {
    //files must be loaded into memory to manipulate, but
    //one or more files exceed the memory limit setting
      return false;
    }
  uint32 size;
  uint8 *data;
  //remove header, if it exists
    data = load(fn_x, size);
    if((size & 0x1fff) == 0x0200) {
      original.open(ACCESS_READ, size - 512, data + 512);
    } else {
      original.open(ACCESS_READ, size, data);
    }
    SafeFree(data);

  //remove header, if it exists
    data = load(fn_y, size);
    if((size & 0x1fff) == 0x0200) {
      modified.open(ACCESS_READ, size - 512, data + 512);
    } else {
      modified.open(ACCESS_READ, size, data);
    }
    SafeFree(data);
  } break;
  }
bool result = create_patch(format, fn_patch);
  original.close();
  modified.close();
  return result;
}

bool BPF::apply_patch(const char *fn_patch, const char *fn_input) {
uint32 size_p = fsize(fn_patch);
uint32 size_i = fsize(fn_input);
bool   lim_p  = (size_p >= settings.memory_limit);
bool   lim_i  = (size_i >= settings.memory_limit);
  if(lim_p == false) {
    patch.load(fn_patch);
  } else {
    patch.open(ACCESS_READ, fn_patch);
  }

  patch.seek(INDEX_FORMAT);
uint32 format;
  format  = patch.read();
  format |= patch.read() << 8;
  format |= patch.read() << 16;
  format |= patch.read() << 24;
  patch.seek(0);

  switch(format) {
  case FORMAT_BINARY: {
    if(lim_i == false) {
      input.load(fn_input);
    } else {
      input.open(ACCESS_READ, fn_input);
    }
  } break;
  case FORMAT_SNES: {
    if(lim_i == true) {
    //file too large to load into memory?
      patch.close();
      return false;
    }
  uint32 size;
  uint8 *data = load(fn_input, size);
  //remove header, if it exists
    if((size & 0x1fff) == 0x0200) {
      input.open(ACCESS_READ, size - 512, data + 512);
    } else {
      input.open(ACCESS_READ, size, data);
    }
    SafeFree(data);
  } break;
  }
bool result = apply_patch();
  patch.close();
  input.close();
  return result;
}

bool BPF::apply_patch(uint32 s_patch, uint8 *p_patch, uint32 s_input, uint8 *p_input) {
  patch.open(ACCESS_READ, s_patch, p_patch);

  patch.seek(INDEX_FORMAT);
uint32 format;
  format  = patch.read();
  format |= patch.read() << 8;
  format |= patch.read() << 16;
  format |= patch.read() << 24;
  patch.seek(0);

  switch(format) {
  case FORMAT_BINARY: {
    input.open(ACCESS_READ, s_input, p_input);
  } break;
  case FORMAT_SNES: {
  //remove header, if it exists
    if((s_input & 0x1fff) == 0x0200) {
      input.open(ACCESS_READ, s_input - 512, p_input + 512);
    } else {
      input.open(ACCESS_READ, s_input, p_input);
    }
  } break;
  }
bool result = apply_patch();
  patch.close();
  input.close();
  return result;
}

uint8 *BPF::get_output_handle(uint32 &size) {
  size = output.size;
  return (uint8*)output.data.handle();
}

void BPF::save_output(const char *fn) {
  output.save(fn);
}

void BPF::clear_settings() {
  settings.memory_limit = (16 * 1024 * 1024) + 4096;

  settings.insert_data.enabled = false;
  settings.insert_data.mode    = 0;
  settings.insert_data.handle  = 0;
  settings.insert_data.data    = 0;
  settings.insert_data.size    = 0;
  strcpy(settings.insert_data.fn, "");
}

BPF::BPF() {
  clear_settings();
  original.set_parent(this);
  modified.set_parent(this);
  output.set_parent(this);
  input.set_parent(this);
  patch.set_parent(this);
}

BPF::~BPF() {
  original.close();
  modified.close();
  output.close();
  input.close();
  patch.close();
}
