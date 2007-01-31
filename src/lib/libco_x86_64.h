/*
  libco_x86_64 : version 0.09 ~byuu (2007-01-19)
*/

#define COTHREAD_STACKSIZE_TINY      0x2000
#define COTHREAD_STACKSIZE_SMALL     0x8000
#define COTHREAD_STACKSIZE_NORMAL   0x20000
#define COTHREAD_STACKSIZE_LARGE    0x80000
#define COTHREAD_STACKSIZE_HUGE    0x200000

typedef void (*cothread_t);
typedef void (*cothread_p)(void);

extern "C" cothread_t co_active();
extern "C" cothread_t co_create(cothread_p coentry, unsigned int heapsize);
extern "C" void       co_delete(cothread_t cothread);
extern "C" void       co_switch(cothread_t cothread);
extern "C" void       co_exit(cothread_t cothread);
