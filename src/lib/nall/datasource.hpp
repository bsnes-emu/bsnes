#ifndef NALL_DATASOURCE_HPP
#define NALL_DATASOURCE_HPP

#include <string.h>
#include <nall/file.hpp>
#include <nall/property.hpp>
#include <nall/stdint.hpp>

namespace nall {
  struct datasource : public property {
    enum source_t { disk, memory };

    property_t<source_t> source;
    property_t<char*>    name;
    property_t<uint8_t*> data;
    property_t<unsigned> size;

    datasource(const char *name_) {
      set(source, disk);
      set(name, strdup(name_));
      set(data, (uint8_t*)0);
      set(size, file::size(name_));
    }

    datasource(uint8_t *data_, unsigned size_) {
      set(source, memory);
      set(name, (char*)0);
      set(data, data_);
      set(size, size_);
    }

    ~datasource() {
      if(name()) free(name());
    }
  };
}

#endif
