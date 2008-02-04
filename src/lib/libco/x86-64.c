/*
  libco.x86-64 (2008-01-28)
  author: byuu
  license: public domain
*/

#define LIBCO_C
#include "../libco.h"
#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static thread_local long co_active_buffer[32];
static thread_local cothread_t co_active_ = 0;

static void crash() {
  assert(0); /* called only if cothread_t entrypoint returns */
}

cothread_t co_active() {
  if(!co_active_) co_active_ = &co_active_buffer;
  return co_active_;
}

cothread_t co_create(unsigned int size, void (*entrypoint)(void)) {
  cothread_t handle;
  assert(sizeof(long) == 8);
  if(!co_active_) co_active_ = &co_active_buffer;
  size += 128; /* allocate additional space for storage */
  size &= ~15; /* align stack to 16-byte boundary */

  if(handle = (cothread_t)calloc(size, 1)) {
    long *p = (long*)((char*)handle + size); /* seek to top of stack */
    *--p = (long)crash;                      /* crash if entrypoint returns */
    *--p = (long)entrypoint;                 /* start of function */
    *(long*)handle = (long)p;                /* stack pointer */
  }

  return handle;
}

void co_delete(cothread_t handle) {
  free(handle);
}

void co_switch(cothread_t to) {
  register long stack = *(long*)to; /* stack[0] = "to" thread entry point */
  register cothread_t from = co_active_;
  co_active_ = to;

  __asm__ __volatile__(
    "movq %%rsp,(%1)            \n\t" /* save old stack pointer */
    "movq (%0),%%rsp            \n\t" /* load new stack pointer */
    "addq $8,%%rsp              \n\t" /* "pop" return address off stack */

    "movq %%rbp, 8(%1)          \n\t" /* backup non-volatile registers */
    "movq %%rbx,16(%1)          \n\t"
    "movq %%r12,24(%1)          \n\t"
    "movq %%r13,32(%1)          \n\t"
    "movq %%r14,40(%1)          \n\t"
    "movq %%r15,48(%1)          \n\t"

    "movq  8(%0),%%rbp          \n\t" /* restore non-volatile registers */
    "movq 16(%0),%%rbx          \n\t"
    "movq 24(%0),%%r12          \n\t"
    "movq 32(%0),%%r13          \n\t"
    "movq 40(%0),%%r14          \n\t"
    "movq 48(%0),%%r15          \n\t"

    "jmp  *(%2)                 \n\t" /* jump into "to" thread */
    : /* no outputs */
    : "r" (to), "r" (from), "r" (stack)
  );
}

#ifdef __cplusplus
}
#endif
