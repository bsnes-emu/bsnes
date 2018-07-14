#pragma once

//shared functionality used for pObject on all platforms

struct mLock {
  auto locked() const -> bool { return locks || Application::state.quit; }
  auto lock() -> void { ++locks; }
  auto unlock() -> void { --locks; }

  struct Handle {
    Handle(const mLock& self) : self(self) { ++self.locks; }
    ~Handle() { --self.locks; }
    const mLock& self;
  };
  auto acquire() const -> Handle { return {*this}; }

  mutable int locks = 0;
};
