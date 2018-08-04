#pragma once

//shared functionality used for pObject on all platforms

struct mLock {
  struct Handle {
    Handle(const mLock* self) : self(self) {
      if(self) {
        ++self->locks;
      }
    }

    ~Handle() {
      release();
    }

    auto release() -> bool {
      if(self) {
        --self->locks;
        self = nullptr;
        return true;
      }
      return false;
    }

    const mLock* self = nullptr;
  };

  auto acquired() const -> bool {
    return locks || Application::state.quit;
  }

  auto acquire() const -> Handle {
    return {this};
  }

  //deprecated C-style manual functions
  //prefer RAII acquire() functionality instead in newly written code
  auto locked() const -> bool {
    return acquired();
  }

  auto lock() -> void {
    ++locks;
  }

  auto unlock() -> void {
    --locks;
  }

  mutable int locks = 0;
};

using Lock = mLock;
