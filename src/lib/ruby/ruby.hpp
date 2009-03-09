/*
  ruby
  version: 0.04 (2008-08-20)
  license: public domain
*/

#ifndef RUBY_H
#define RUBY_H

#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/input.hpp>
#include <nall/new.hpp>
#include <nall/stdint.hpp>

namespace ruby {

#include <ruby/video.hpp>
#include <ruby/audio.hpp>
#include <ruby/input.hpp>

class VideoInterface {
public:
  void driver(const char *driver = "");
  const char* default_driver();
  const char* driver_list();
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
  const char* default_driver();
  const char* driver_list();
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

  unsigned volume;

  //resample unit
  double hermite(double mu, double a, double b, double c, double d);
  bool   resample_enabled;
  double r_outfreq, r_infreq;
  double r_step, r_frac;
  int    r_left[4], r_right[4];
};

class InputInterface {
public:
  void driver(const char *driver = "");
  const char* default_driver();
  const char* driver_list();
  bool init();
  void term();

  bool cap(Input::Setting setting);
  uintptr_t get(Input::Setting setting);
  bool set(Input::Setting setting, uintptr_t param);

  bool acquire();
  bool unacquire();
  bool acquired();

  bool poll(int16_t *table);
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
