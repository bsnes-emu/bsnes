/*
  ruby
  version: 0.01 (2008-02-03)
  license: public domain
*/

#ifndef RUBY_H
#define RUBY_H

#include <nall/algorithm.hpp>
#include <nall/input.hpp>
#include <nall/new.hpp>
#include <nall/stdint.hpp>
using nall::min;
using nall::max;
using nall::zeromemory;

namespace ruby {

#include <ruby/video.h>
#include <ruby/audio.h>
#include <ruby/input.h>

class VideoInterface {
public:
  void driver(const char *driver = "");
  bool init();
  void term();

  bool cap(Video::Setting setting);
  uintptr_t get(Video::Setting setting);
  bool set(Video::Setting setting, uintptr_t param);
  bool lock(uint32_t *&data, unsigned &pitch);
  void unlock();
  void clear();
  void refresh(unsigned width, unsigned height);
  VideoInterface();
  ~VideoInterface();

private:
  Video *p;
};

class AudioInterface {
public:
  void driver(const char *driver = "");
  bool init();
  void term();

  bool cap(Audio::Setting setting);
  uintptr_t get(Audio::Setting setting);
  bool set(Audio::Setting setting, uintptr_t param);
  void sample(uint16_t left, uint16_t right);
  void clear();
  AudioInterface();
  ~AudioInterface();

private:
  Audio *p;
};

class InputInterface {
public:
  void driver(const char *driver = "");
  bool init();
  void term();

  bool cap(Input::Setting setting);
  uintptr_t get(Input::Setting setting);
  bool set(Input::Setting setting, uintptr_t param);
  bool key_down(uint16_t key);
  bool key_up(uint16_t key);
  void clear();
  void poll();
  InputInterface();
  ~InputInterface();

private:
  Input *p;
};

extern VideoInterface video;
extern AudioInterface audio;
extern InputInterface input;

} //namespace ruby

#endif //ifndef RUBY_H
