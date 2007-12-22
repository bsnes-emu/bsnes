/*
  libco
  version: 0.11 (2007-12-11)
  license: public domain
*/

#ifndef LIBCO_H
#define LIBCO_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__i386__)
  #if defined(__GNUC__)
    #define calltype __attribute__((fastcall))
  #elif defined(_MSC_VER)
    #define calltype __fastcall
  #endif
#else
  #define calltype
#endif

typedef void* cothread_t;

cothread_t calltype co_active();
cothread_t calltype co_create(unsigned int heapsize, void (*coentry)());
void calltype co_delete(cothread_t cothread);
void calltype co_switch(cothread_t cothread);

#undef calltype

#ifdef __cplusplus
} //extern "C"
#endif

#endif //ifndef LIBCO_H
