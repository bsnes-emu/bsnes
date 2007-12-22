/*
  libco.ucontext (2007-09-08)
  author: byuu
  license: public domain
*/

#include <stdlib.h>
#include <ucontext.h>
#include "../libco.h"

//WARNING: the overhead of POSIX ucontext is very high,
//averaging ~450x that of standard subroutine calls.
//(tested on FreeBSD 6.2-RELEASE)
//By contrast, on the same system, libco_x86's overhead
//is ~7.25x standard subroutine calls; or fifty times faster.
//
//This library only exists for two reasons:
//1 - as an initial test for the viability of a ucontext implementation
//2 - to demonstrate the power and speed of libco over existing implementations,
//    such as pth (which defaults to wrapping ucontext on unix targets)
//
//Use this library only as a *last resort*

struct cothread_struct {
  ucontext_t cohandle;
  void (*coentry)();
};

cothread_t __co_active = 0, __co_primary = 0;
void co_entrypoint(cothread_t cothread);
void co_init();

/*****
 * library functions
 *****/

cothread_t co_active() {
  if(__co_primary == 0)co_init();
  return __co_active;
}

cothread_t co_create(unsigned int heapsize, void (*coentry)()) {
  if(__co_primary == 0)co_init();
cothread_struct *thread = (cothread_struct*)malloc(sizeof(cothread_struct));
  thread->coentry = coentry;
  getcontext(&thread->cohandle);
  heapsize += 512;
  thread->cohandle.uc_stack.ss_sp = (char*)malloc(heapsize);
  thread->cohandle.uc_stack.ss_size = heapsize;
  makecontext(&thread->cohandle, (void (*)())co_entrypoint, 1, thread);
  return (cothread_t)thread;
}

void co_delete(cothread_t cothread) {
cothread_struct *thread = (cothread_struct*)cothread;
  free(thread->cohandle.uc_stack.ss_sp);
  free(thread);
}

void co_switch(cothread_t cothread) {
cothread_struct *active = (cothread_struct*)__co_active;
cothread_struct *swap = (cothread_struct*)cothread;
  __co_active = cothread;
  swapcontext(&active->cohandle, &swap->cohandle);
}

/*****
 * internal functions
 *****/

void co_entrypoint(cothread_t cothread) {
  ((cothread_struct*)cothread)->coentry();
}

void co_init() {
cothread_struct *thread = (cothread_struct*)malloc(sizeof(cothread_struct));
  thread->coentry = 0;
  getcontext(&thread->cohandle);
  __co_active = __co_primary = (cothread_t)thread;
}
