#ifndef GAMBATTE_MEMORYINTERFACE_H
#define GAMBATTE_MEMORYINTERFACE_H

namespace Gambatte {

struct MemoryBuffer {
	void *data;
	unsigned size;
};

class MemoryInterface {
public:
	virtual MemoryBuffer loadRomData() = 0;
	virtual MemoryBuffer loadRamData() = 0;
	virtual MemoryBuffer loadRtcData() = 0;

	virtual MemoryBuffer saveRamData(unsigned size) = 0;
	virtual MemoryBuffer saveRtcData() = 0;

	virtual void joypWrite(bool /*p15*/, bool /*p14*/) {}
};

}

#endif
