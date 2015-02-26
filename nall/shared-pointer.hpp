#ifndef NALL_SHARED_POINTER_HPP
#define NALL_SHARED_POINTER_HPP

#include <nall/function.hpp>
#include <nall/maybe.hpp>
#include <nall/traits.hpp>
#include <nall/vector.hpp>

namespace nall {

template<typename T> struct shared_pointer;

struct shared_pointer_manager {
  void* pointer = nullptr;
  function<void (void*)> deleter;
  unsigned strong = 0;
  unsigned weak = 0;

  shared_pointer_manager(void* pointer) : pointer(pointer) {
  }
};

template<typename T> struct shared_pointer;
template<typename T> struct shared_pointer_weak;

template<typename T>
struct shared_pointer {
  using type = T;
  shared_pointer_manager* manager = nullptr;

  template<typename U>
  struct is_compatible {
    static constexpr bool value = is_base_of<T, U>::value || is_base_of<U, T>::value;
  };

  shared_pointer() {
  }

  shared_pointer(T* source) {
    operator=(source);
  }

  shared_pointer(T* source, const function<void (T*)>& deleter) {
    operator=(source);
    manager->deleter = [=](void* p) { deleter((T*)p); };
  }

  shared_pointer(const shared_pointer& source) {
    operator=(source);
  }

  shared_pointer(shared_pointer&& source) {
    operator=(move(source));
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer(const shared_pointer<U>& source) {
    operator=<U>(source);
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer(shared_pointer<U>&& source) {
    operator=<U>(move(source));
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer(const shared_pointer_weak<U>& source) {
    operator=<U>(source);
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer(const shared_pointer<U>& source, T* pointer) {
    if((bool)source && (T*)source.manager->pointer == pointer) {
      manager = source.manager;
      manager->strong++;
    }
  }

  ~shared_pointer() {
    reset();
  }

  shared_pointer& operator=(T* source) {
    reset();
    if(source) {
      manager = new shared_pointer_manager((void*)source);
      manager->strong++;
    }
    return *this;
  }

  shared_pointer& operator=(const shared_pointer& source) {
    if(this != &source) {
      reset();
      if((bool)source) {
        manager = source.manager;
        manager->strong++;
      }
    }
    return *this;
  }

  shared_pointer& operator=(shared_pointer&& source) {
    if(this != &source) {
      reset();
      manager = source.manager;
      source.manager = nullptr;
    }
    return *this;
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer& operator=(const shared_pointer<U>& source) {
    if((uintptr_t)this != (uintptr_t)&source) {
      reset();
      if((bool)source) {
        manager = source.manager;
        manager->strong++;
      }
    }
    return *this;
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer& operator=(shared_pointer&& source) {
    if((uintptr_t)this != (uintptr_t)&source) {
      reset();
      manager = source.manager;
      source.manager = nullptr;
    }
    return *this;
  }

  template<typename U, typename = enable_if<is_compatible<U>>>
  shared_pointer& operator=(const shared_pointer_weak<U>& source) {
    reset();
    if((bool)source) {
      manager = source.manager;
      manager->strong++;
    }
    return *this;
  }

  T* data() {
    if(manager) return (T*)manager->pointer;
    return nullptr;
  }

  const T* data() const {
    if(manager) return (T*)manager->pointer;
    return nullptr;
  }

  T* operator->() { return data(); }
  const T* operator->() const { return data(); }

  T& operator*() { return *data(); }
  const T& operator*() const { return *data(); }

  T& operator()() { return *data(); }
  const T& operator()() const { return *data(); }

  template<typename U>
  bool operator==(const shared_pointer<U>& source) const {
    return manager == source.manager;
  }

  template<typename U>
  bool operator!=(const shared_pointer<U>& source) const {
    return manager != source.manager;
  }

  explicit operator bool() const {
    return !empty();
  }

  bool empty() const {
    return !manager || !manager->strong;
  }

  bool unique() const {
    return manager && manager->strong == 1;
  }

  void reset() {
    if(manager && manager->strong) {
      //pointer may contain weak references; if strong==0 it may destroy manager
      //as such, we must destroy strong before decrementing it to zero
      if(manager->strong == 1) {
        if(manager->deleter) {
          manager->deleter(manager->pointer);
        } else {
          delete (T*)manager->pointer;
        }
        manager->pointer = nullptr;
      }
      if(--manager->strong == 0) {
        if(manager->weak == 0) {
          delete manager;
        }
      }
    }
    manager = nullptr;
  }

  template<typename U>
  shared_pointer<U> cast() {
    if(auto pointer = dynamic_cast<U*>(data())) {
      return {*this, pointer};
    }
    return {};
  }
};

template<typename T>
struct shared_pointer_weak {
  using type = T;
  shared_pointer_manager* manager = nullptr;

  shared_pointer_weak() {
  }

  shared_pointer_weak(const shared_pointer<T>& source) {
    operator=(source);
  }

  shared_pointer_weak& operator=(const shared_pointer<T>& source) {
    reset();
    if(manager = source.manager) manager->weak++;
    return *this;
  }

  ~shared_pointer_weak() {
    reset();
  }

  explicit operator bool() const {
    return !empty();
  }

  bool empty() const {
    return !manager || !manager->strong;
  }

  shared_pointer<T> acquire() const {
    return shared_pointer<T>(*this);
  }

  void reset() {
    if(manager && --manager->weak == 0) {
      if(manager->strong == 0) {
        delete manager;
      }
    }
    manager = nullptr;
  }
};

template<typename T>
struct shared_pointer_new : shared_pointer<T> {
  template<typename... P>
  shared_pointer_new(P&&... p) : shared_pointer<T>(new T(forward<P>(p)...)) {
  }
};

}

#endif
