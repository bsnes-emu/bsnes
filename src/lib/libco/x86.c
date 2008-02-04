/*
  libco.x86 (2008-01-28)
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
  assert(sizeof(long) == 4);
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

#if defined(__GNUC__)

void co_switch(cothread_t to) {
  register long stack = *(long*)to; /* stack[0] = "to" thread entry point */
  register cothread_t from = co_active_;
  co_active_ = to;

  __asm__ __volatile__(
    "movl %%esp,(%1)            \n\t" /* save old stack pointer */
    "movl (%0),%%esp            \n\t" /* load new stack pointer */
    "addl $4,%%esp              \n\t" /* "pop" return address off stack */

    "movl %%ebp, 4(%1)          \n\t" /* backup non-volatile registers */
    "movl %%esi, 8(%1)          \n\t"
    "movl %%edi,12(%1)          \n\t"
    "movl %%ebx,16(%1)          \n\t"

    "movl  4(%0),%%ebp          \n\t" /* restore non-volatile registers */
    "movl  8(%0),%%esi          \n\t"
    "movl 12(%0),%%edi          \n\t"
    "movl 16(%0),%%ebx          \n\t"

    "jmp  *(%2)                 \n\t" /* jump into "to" thread */
    : /* no outputs */
    : "r" (to), "r" (from), "r" (stack)
  );
}

#elif defined(_MSC_VER)

__declspec(naked) __declspec(noinline)
static void __fastcall co_swap(register cothread_t to, register cothread_t from) {
  /* ecx = to, edx = from */
  __asm {
    mov [edx],esp
    mov esp,[ecx]
    pop eax

    mov [edx+ 4],ebp
    mov [edx+ 8],esi
    mov [edx+12],edi
    mov [edx+16],ebx

    mov ebp,[ecx+ 4]
    mov esi,[ecx+ 8]
    mov edi,[ecx+12]
    mov ebx,[ecx+16]

    jmp eax
  }
}

void co_switch(cothread_t handle) {
  register cothread_t co_prev_ = co_active_;
  co_swap(co_active_ = handle, co_prev_);
}

#endif

#ifdef __cplusplus
}
#endif
