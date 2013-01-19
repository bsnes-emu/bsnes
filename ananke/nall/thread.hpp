#ifndef NALL_THREAD_HPP
#define NALL_THREAD_HPP

#include <nall/platform.hpp>
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>

#if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  #include <thread>
#elif defined(PLATFORM_WIN)

//TDM/GCC 4.7 does not support std::thread
//implement an extremely lightweight wrapper

namespace std {
  inline DWORD WINAPI thread_entry_point(LPVOID parameter);

  struct thread {
    bool joinable() const {
      return active == false;
    }

    void join() {
      while(active) usleep(1);
    }

    thread(function<void ()> entryPoint) : entryPoint(entryPoint), active(true) {
      CreateThread(NULL, 0, thread_entry_point, (void*)this, 0, NULL);
    }

  private:
    function<void ()> entryPoint;
    bool active;
    friend DWORD WINAPI thread_entry_point(LPVOID);
  };

  inline DWORD WINAPI thread_entry_point(LPVOID parameter) {
    thread *context = (thread*)parameter;
    context->entryPoint();
    context->active = false;
    return 0;
  }
}
#endif

#endif
