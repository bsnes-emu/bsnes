#include "libbase.h"
#include "libvector.h"
#include "libfile.h"

/*****
 * c wrappers
 *****/

void   fread(file *s, uint8 *data, uint length) { s->read(data, length); }
uint8  fread(file *s) { return s->read(); }
uint8  fgetc(file *s) { return s->read(); }

void   fwrite(file *s, uint8 *data, uint length) { s->write(data, length); }
void   fwrite(file *s, uint8 data) { s->write(data); }
void   fputc(file *s, uint8 data) { s->write(data); }

void   fseek(file *s, uint offset, uint mode) { s->seek(offset, mode); }
uint   foffset(file *s) { return s->offset(); }
uint   ftell(file *s) { return s->offset(); }
uint   fsize(file *s) { return s->size(); }
bool   feof(file *s) { return s->eof(); }

bool   fopen(file *s, const char *filename, uint mode) { return s->open(filename, mode); }
bool   fopen(file *s) { return s->open(); }
bool   fflush(file *s) { return s->flush(); }
bool   fclose(file *s) { return s->close(); }

/*****
 * c++ wrappers
 *****/

void   fread(file &s, uint8 *data, uint length) { s.read(data, length); }
uint8  fread(file &s) { return s.read(); }
uint8  fgetc(file &s) { return s.read(); }

void   fwrite(file &s, uint8 *data, uint length) { s.write(data, length); }
void   fwrite(file &s, uint8 data) { s.write(data); }
void   fputc(file &s, uint8 data) { s.write(data); }

void   fseek(file &s, uint offset, uint mode) { s.seek(offset, mode); }
uint   foffset(file &s) { return s.offset(); }
uint   ftell(file &s) { return s.offset(); }
uint   fsize(file &s) { return s.size(); }
bool   feof(file &s) { return s.eof(); }

bool   fopen(file &s, const char *filename, uint mode) { return s.open(filename, mode); }
bool   fopen(file &s) { return s.open(); }
bool   fflush(file &s) { return s.flush(); }
bool   fclose(file &s) { return s.close(); }
