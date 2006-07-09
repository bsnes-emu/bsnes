/*
  libco_x86 : version 0.06 ~byuu (05/21/2006)
*/

typedef void (*thread_t);
typedef void (*thread_p)();

extern "C" void     __fastcall co_init();
extern "C" void     __fastcall co_term();
extern "C" thread_t __fastcall co_active();
extern "C" thread_t __fastcall co_create(thread_p coentry, unsigned int heapsize);
extern "C" void     __fastcall co_delete(thread_t cothread);
extern "C" void     __fastcall co_jump(thread_t cothread);
extern "C" void     __fastcall co_call(thread_t cothread);
extern "C" void     __fastcall co_return();
