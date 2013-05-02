#ifndef NALL_THREAD_HPP
#define NALL_THREAD_HPP

#include <nall/platform.hpp>
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>

#if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  #include <pthread.h>

namespace nall {

inline void* thread_entry_point(void*);

struct thread {
  thread(function<void ()> entryPoint) : entryPoint(entryPoint), completed(false), dead(false) {
    initialize();
    pthread_create(&pthread, nullptr, thread_entry_point, (void*)this);
  }

  ~thread() {
    join();
  }

  bool active() const {
    return completed == false;
  }

  void join() {
    if(dead) return;
    dead = true;
    pthread_join(pthread, nullptr);
  }

  static bool primary() {
    initialize();
    return pthread_equal(primaryThread(), pthread_self());
  }

private:
  pthread_t pthread;
  function<void ()> entryPoint;
  volatile bool completed, dead;
  friend void* thread_entry_point(void*);

  static void initialize() {
    static bool initialized = false;
    if(initialized) return;
    initialized = true;
    primaryThread() = pthread_self();
  }

  static pthread_t& primaryThread() {
    static pthread_t thread;
    return thread;
  }
};

void* thread_entry_point(void* parameter) {
  thread* context = (thread*)parameter;
  context->entryPoint();
  context->completed = true;
  pthread_exit(nullptr);
}

}
#elif defined(PLATFORM_WIN)
namespace nall {

inline DWORD WINAPI thread_entry_point(LPVOID);

struct thread {
  thread(function<void ()> entryPoint) : entryPoint(entryPoint), completed(false), dead(false) {
    initialize();
    hthread = CreateThread(nullptr, 0, thread_entry_point, (void*)this, 0, nullptr);
  }

  ~thread() {
    join();
  }

  bool active() const {
    return completed == false;
  }

  void join() {
    if(dead) return;
    dead = true;
    WaitForSingleObject(hthread, INFINITE);
    CloseHandle(hthread);
  }

  static bool primary() {
    initialize();
    return primaryThread() == GetCurrentThreadId();
  }

private:
  HANDLE hthread;
  function<void ()> entryPoint;
  volatile bool completed, dead;
  friend DWORD WINAPI thread_entry_point(LPVOID);

  static void initialize() {
    static bool initialized = false;
    if(initialized) return;
    initialized = true;
    primaryThread() = GetCurrentThreadId();
  }

  static DWORD& primaryThread() {
    static DWORD thread;
    return thread;
  }
};

inline DWORD WINAPI thread_entry_point(LPVOID parameter) {
  thread *context = (thread*)parameter;
  context->entryPoint();
  context->completed = true;
  return 0;
}

}
#endif

#endif
