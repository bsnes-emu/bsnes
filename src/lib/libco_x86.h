/*
  libco_x86 : version 0.08 ~byuu (10/21/2006)
*/

#define COTHREAD_STACKSIZE_TINY      0x1000
#define COTHREAD_STACKSIZE_SMALL     0x4000
#define COTHREAD_STACKSIZE_NORMAL   0x10000
#define COTHREAD_STACKSIZE_LARGE    0x40000
#define COTHREAD_STACKSIZE_HUGE    0x100000

typedef void (*cothread_t);
typedef void (*cothread_p)(void);

extern "C" cothread_t fastcall co_init();
extern "C" void       fastcall co_term();
extern "C" cothread_t fastcall co_active();
extern "C" cothread_t fastcall co_create(cothread_p coentry, unsigned int heapsize);
extern "C" void       fastcall co_delete(cothread_t cothread);
extern "C" void       fastcall co_switch(cothread_t cothread);
extern "C" void       fastcall co_exit(cothread_t cothread);
