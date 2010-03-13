#include <stdint.h>
class QWidget;
namespace nall { class configuration; }

extern "C" {
  const char* snesfilter_supported();
  void snesfilter_configuration(nall::configuration&);
  void snesfilter_colortable(const uint32_t*);
  void snesfilter_size(unsigned, unsigned&, unsigned&, unsigned, unsigned);
  void snesfilter_render(
    unsigned, uint32_t*, unsigned,
    const uint16_t*, unsigned,
    const unsigned*, unsigned, unsigned
  );
  QWidget* snesfilter_settings(unsigned);
}
