#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {
namespace Markup {

inline Node Document(const string& markup) {
  if(markup.beginswith("<")) return XML::Document(markup);
  return BML::Document(markup);
}

}
}

#endif
