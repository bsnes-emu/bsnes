/*
  libco_x86 : version 0.10 ~byuu (2007-06-04)
  license: public domain
*/

#ifndef LIBCO_H
#define LIBCO_H

#undef fastcall
#if defined(_MSC_VER)
  #define fastcall __fastcall
#elif defined(__GNUC__)
  #define fastcall __attribute__((fastcall))
#else
  #error "fastcall undefined"
#endif

#define cocall fastcall
typedef void (*cothread_t);

extern "C" cothread_t cocall co_active();
extern "C" cothread_t cocall co_create(unsigned int heapsize, void (cocall *coentry)(void *data), void *data);
extern "C" void       cocall co_delete(cothread_t cothread);
extern "C" void       cocall co_switch(cothread_t cothread);

#endif
