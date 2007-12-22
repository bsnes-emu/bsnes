/*
  libco.win (2007-09-08)
  author: byuu
  license: public domain
*/

#define WINVER 0x0400
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include "../libco.h"

struct cothread_struct {
  void *cohandle;
  void (*coentry)();
};

cothread_t __co_active = 0, __co_primary = 0;

void __stdcall co_entryproc(void*);
cothread_t co_init();

/*****
 * library functions
 *****/

cothread_t co_active() {
  if(__co_primary == 0)co_init();
  return __co_active;
}

cothread_t co_create(unsigned int heapsize, void (*coentry)()) {
  if(__co_primary == 0)co_init();
cothread_struct *s = (cothread_struct*)malloc(sizeof(cothread_struct));
  s->coentry  = coentry;
  s->cohandle = CreateFiber(heapsize + 512, co_entryproc, (void*)s);
  return (cothread_t)s;
}

void co_delete(cothread_t cothread) {
cothread_struct *s = (cothread_struct*)cothread;
  DeleteFiber(s->cohandle);
  free(cothread);
}

void co_switch(cothread_t cothread) {
  __co_active = cothread;
cothread_struct *s = (cothread_struct*)cothread;
  SwitchToFiber(s->cohandle);
}

/*****
 * internal functions
 *****/

void __stdcall co_entryproc(void *cothread) {
  ((cothread_struct*)cothread)->coentry();
}

cothread_t co_init() {
  ConvertThreadToFiber(0);
cothread_struct *s = (cothread_struct*)malloc(sizeof(cothread_struct));
  s->coentry  = 0;
  s->cohandle = GetCurrentFiber();
  __co_active = __co_primary = (cothread_t)s;
  return __co_active;
}
