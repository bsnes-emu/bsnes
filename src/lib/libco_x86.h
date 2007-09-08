/*
  libco_x86 : version 0.10 ~byuu (2007-09-08)
  license: public domain
*/

#ifndef LIBCO_H
#define LIBCO_H

#if !defined(fastcall)
  #if defined(_MSC_VER)
    #define fastcall __fastcall
  #elif defined(__GNUC__)
    #define fastcall __attribute__((fastcall))
  #else
    #error "fastcall undefined"
  #endif
#endif

typedef void *cothread_t;

extern "C" cothread_t fastcall co_active();
extern "C" cothread_t fastcall co_create(unsigned int heapsize, void (*coentry)());
extern "C" void       fastcall co_delete(cothread_t cothread);
extern "C" void       fastcall co_switch(cothread_t cothread);

#endif
