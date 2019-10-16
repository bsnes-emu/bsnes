#define LIBCO_C
#include "libco.h"
#include "settings.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

static thread_local unsigned long co_active_buffer[64];
static thread_local cothread_t co_active_handle = 0;
static void (*co_swap)(cothread_t, cothread_t) = 0;

#ifdef LIBCO_MPROTECT
  alignas(4096)
#else
  section(text)
#endif
static const uint32_t co_swap_function[1024] = {
  0xa9002428,  /* stp x8,x9,[x1]        */
  0xa9012c2a,  /* stp x10,x11,[x1,#16]  */
  0xa902342c,  /* stp x12,x13,[x1,#32]  */
  0xa9033c2e,  /* stp x14,x15,[x1,#48]  */
  0xf9002433,  /* str x19,[x1,#72]      */
  0xa9055434,  /* stp x20,x21,[x1,#80]  */
  0xa9065c36,  /* stp x22,x23,[x1,#96]  */
  0xa9076438,  /* stp x24,x25,[x1,#112] */
  0xa9086c3a,  /* stp x26,x27,[x1,#128] */
  0xa909743c,  /* stp x28,x29,[x1,#144] */
  0x910003f0,  /* mov x16,sp            */
  0xa90a7830,  /* stp x16,x30,[x1,#160] */

  0xa9402408,  /* ldp x8,x9,[x0]        */
  0xa9412c0a,  /* ldp x10,x11,[x0,#16]  */
  0xa942340c,  /* ldp x12,x13,[x0,#32]  */
  0xa9433c0e,  /* ldp x14,x15,[x0,#48]  */
  0xf9402413,  /* ldr x19,[x0,#72]      */
  0xa9455414,  /* ldp x20,x21,[x0,#80]  */
  0xa9465c16,  /* ldp x22,x23,[x0,#96]  */
  0xa9476418,  /* ldp x24,x25,[x0,#112] */
  0xa9486c1a,  /* ldp x26,x27,[x0,#128] */
  0xa949741c,  /* ldp x28,x29,[x0,#144] */
  0xa94a4410,  /* ldp x16,x17,[x0,#160] */
  0x9100021f,  /* mov sp,x16            */
  0xd61f0220,  /* br x17                */
};

static void co_init() {
  #ifdef LIBCO_MPROTECT
  unsigned long addr = (unsigned long)co_swap_function;
  unsigned long base = addr - (addr % sysconf(_SC_PAGESIZE));
  unsigned long size = (addr - base) + sizeof co_swap_function;
  mprotect((void*)base, size, PROT_READ | PROT_EXEC);
  #endif
}

const char* co_method() {
  return "aarch64";
}

cothread_t co_active() {
  if(!co_active_handle) co_active_handle = &co_active_buffer;
  return co_active_handle;
}

cothread_t co_derive(void* memory, unsigned int size, void (*entrypoint)(void)) {
  unsigned long* handle;
  if(!co_swap) {
    co_init();
    co_swap = (void (*)(cothread_t, cothread_t))co_swap_function;
  }
  if(!co_active_handle) co_active_handle = &co_active_buffer;

  if(handle = (unsigned long*)memory) {
    unsigned int offset = (size & ~15);
    unsigned long* p = (unsigned long*)((unsigned char*)handle + offset);
    handle[19] = (unsigned long)p;           /* x29 (frame pointer) */
    handle[20] = (unsigned long)p;           /* x30 (stack pointer) */
    handle[21] = (unsigned long)entrypoint;  /* x31 (link register) */
  }

  return handle;
}

cothread_t co_create(unsigned int size, void (*entrypoint)(void)) {
  void* memory = malloc(size);
  if(!memory) return (cothread_t)0;
  return co_derive(memory, size, entrypoint);
}

void co_delete(cothread_t handle) {
  free(handle);
}

void co_switch(cothread_t handle) {
  cothread_t co_previous_handle = co_active_handle;
  co_swap(co_active_handle = handle, co_previous_handle);
}

#ifdef __cplusplus
}
#endif
