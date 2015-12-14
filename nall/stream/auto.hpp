#ifndef NALL_STREAM_AUTO_HPP
#define NALL_STREAM_AUTO_HPP

namespace nall {

#define autostream(...) (*makestream(__VA_ARGS__))

inline auto makestream(const string& path) -> std::unique_ptr<stream> {
  if(path.iendsWith(".gz")) return std::unique_ptr<stream>(new gzipstream(filestream{path}));
  if(path.iendsWith(".zip")) return std::unique_ptr<stream>(new zipstream(filestream{path}));
  return std::unique_ptr<stream>(new mmapstream(path));
}

inline auto makestream(uint8* data, uint size) -> std::unique_ptr<stream> {
  return std::unique_ptr<stream>(new memorystream(data, size));
}

inline auto makestream(const uint8* data, uint size) -> std::unique_ptr<stream> {
  return std::unique_ptr<stream>(new memorystream(data, size));
}

}

#endif
