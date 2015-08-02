#ifndef NALL_BEAT_ARCHIVE_HPP
#define NALL_BEAT_ARCHIVE_HPP

#include <nall/beat/file.hpp>

namespace nall { namespace Beat {

struct Archive {
  static auto create(const string& beatname, const string& pathname, const string& metadata = "") -> bool;
  static auto unpack(const string& beatname, const string& pathname) -> bool;
  static auto extract(const string& beatname, const string& pathname) -> vector<uint8_t>;

private:
  static auto scan(lstring& result, const string& basename, const string& pathname) -> void;
};

auto Archive::create(const string& beatname, const string& pathname, const string& metadata) -> bool {
  if(!beatname.endsWith(".bpa") || !pathname.endsWith("/")) return false;  //protect against reversed arguments

  File beat{beatname, file::mode::write};
  if(!beat) return false;  //file not writable?

  beat.writes("BPA1");
  beat.writevu(metadata.size());
  beat.writes(metadata);

  lstring contents;
  scan(contents, pathname, pathname);

  for(auto& name : contents) {
    string location{pathname, name};
    bool directory = name.endsWith("/");
    bool readable = file_system_object::readable(location);
    bool writable = file_system_object::writable(location);
    bool executable = file_system_object::executable(location);
    unsigned info = directory << 0 | readable << 1 | writable << 2 | executable << 3 | (name.rtrim("/").size() - 1) << 4;

    beat.writevu(info);
    beat.writes(name);
    if(directory) continue;

    File input{location, file::mode::read};
    if(input) {
      auto size = input.size();
      beat.writevu(size);
      while(size--) beat.write(input.read());
      beat.writel(input.checksum.value(), 4);
    } else {
      beat.writevu(0);
      beat.writel(0x00000000, 4);
    }
  }

  beat.writel(beat.checksum.value(), 4);
  return true;
}

auto Archive::unpack(const string& beatname, const string& pathname) -> bool {
  if(!beatname.endsWith(".bpa") || !pathname.endsWith("/")) return false;  //protect against reversed arguments

  File beat{beatname, file::mode::read};
  if(!beat) return false;  //file not readable?

  if(beat.reads(4) != "BPA1") return false;
  auto size = beat.readvu();
  while(size--) beat.read();

  directory::create(pathname);
  while(beat.offset() < beat.size() - 4) {
    auto info = beat.readvu();
    auto name = beat.reads((info >> 4) + 1);
    if(name.find("\\") || name.find("../")) return false;  //block path exploits

    string location{pathname, name};
    bool directory = info & 1;
    bool readable = info & 2;
    bool writable = info & 4;
    bool executable = info & 8;

    if(directory) {
      if(!nall::directory::create(location)) return false;
    } else {
      File output{location, file::mode::write};
      if(!output) return false;

      auto size = beat.readvu();
      while(size--) output.write(beat.read());
      if(beat.readl(4) != output.checksum.value()) return false;
    }
  }

  uint32_t checksum = beat.checksum.value();
  return beat.readl(4) == checksum;
}

auto Archive::extract(const string& beatname, const string& filename) -> vector<uint8_t> {
  File beat{beatname, file::mode::read};
  if(!beat) return {};  //file not readable?

  if(beat.reads(4) != "BPA1") return {};
  auto size = beat.readvu();
  beat.seek(beat.offset() + size);

  while(beat.offset() < beat.size() - 4) {
    auto info = beat.readvu();
    auto name = beat.reads((info >> 4) + 1);
    if(info & 1) continue;  //ignore directories

    auto size = beat.readvu();
    if(name != filename) {
      beat.seek(beat.offset() + size + 4);
      continue;
    }

    vector<uint8_t> result;
    result.resize(size);
    beat.checksum.reset();
    for(auto n : range(size)) result[n] = beat.read();
    uint32_t checksum = beat.checksum.value();
    if(beat.readl(4) != checksum) return {};
    return result;
  }

  return {};
}

auto Archive::scan(lstring& result, const string& basename, const string& pathname) -> void {
  for(auto& name : directory::contents(pathname)) {
    result.append(string{pathname, name}.ltrim(basename, 1L));
    if(name.endsWith("/")) scan(result, basename, {pathname, name});
  }
}

}}

#endif
