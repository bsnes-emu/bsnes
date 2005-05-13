//#define MEMALLOC_DEBUG

FILE *mem_fp;

void meminit() {
#ifdef MEMALLOC_DEBUG
  mem_fp = fopen("memlog.txt", "wb");
#endif
}

void memterm() {
#ifdef MEMALLOC_DEBUG
  fclose(mem_fp);
#endif
}

void *memalloc(uint32 size, char *name, ...) {
#ifdef MEMALLOC_DEBUG
char vname[4096];
va_list args;
  strcpy(vname, "");
  if(name) {
    va_start(args, name);
    vsprintf(vname, name, args);
    va_end(args);
  }
  fprintf(mem_fp, "memalloc : %10d bytes", size);
  if(*vname)fprintf(mem_fp, ", name = \"%s\"", vname);
  fprintf(mem_fp, "\r\n");
#endif
  return (void*)malloc(size);
}

void memfree(void *mem, char *name, ...) {
#ifdef MEMALLOC_DEBUG
char vname[4096];
va_list args;
  strcpy(vname, "");
  if(name) {
    va_start(args, name);
    vsprintf(vname, name, args);
    va_end(args);
  }
  fprintf(mem_fp, "memfree  :");
  if(*vname)fprintf(mem_fp, " name = \"%s\"", vname);
  fprintf(mem_fp, "\r\n");
#endif
  free(mem);
}

void alert(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  MessageBox(0, str, "bsnes", MB_OK);
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);
  if(w_console) {
    w_console->write(str, Console::DEBUG_MESSAGE);
  }
}

uint32 strhex(char *str) {
uint32 r = 0, m = 0;
int i, ssl = strlen(str);
uint8 x;
  for(i=0;i<ssl;i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else if(str[i] >= 'A' && str[i] <= 'F');
    else if(str[i] >= 'a' && str[i] <= 'f');
    else break;
  }
  for(--i;i>=0;i--, m+=4) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else if(x >= 'A' && x <= 'F')x -= 'A' - 0x0a;
    else if(x >= 'a' && x <= 'f')x -= 'a' - 0x0a;
    else return r;
    r |= x << m;
  }
  return r;
}

uint32 strdec(char *str) {
uint32 m = 1;
int i, r = 0, ssl = strlen(str);
uint8 x;
  for(i=0;i<ssl;i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else break;
  }
  for(--i;i>=0;i--, m*=10) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else return r;
    r += x * m;
  }
  return r;
}

uint32 load_file(char *fn, uint8 **buffer) {
FILE *fp;
uint8 *data;
int fsize;
  fp = fopen(fn, "rb");
  if(!fp)return 0;
  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  if(!fsize) {
    fclose(fp);
    return 0;
  }
  fseek(fp, 0, SEEK_SET);
  data = (uint8*)memalloc(fsize);
  fread(data, 1, fsize, fp);
  fclose(fp);
  *buffer = data;
  return fsize;
}

bool save_file(char *fn, uint8 *data, uint32 size) {
FILE *fp;
  fp = fopen(fn, "wb");
  if(!fp)return false;
  fwrite(data, 1, size, fp);
  fclose(fp);
  return true;
}
