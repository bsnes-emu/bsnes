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
