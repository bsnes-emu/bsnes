name := libretro.so
flags += -std=c++17 -fpermissive -Wno-narrowing -Wno-multichar -fopenmp -g -fPIC

objects := libretro $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/libretro.o: target-libretro/libretro.cpp

all: $(objects)
ifeq ($(platform),linux)
	$(strip $(compiler) -o out/bsnes_libretro.so -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -Wl,-Bdynamic -lpthread -ldl -lgomp)
else ifeq ($(platform),windows)
	$(strip $(compiler) -o out/bsnes_libretro.dll -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -static-libgcc -static-libstdc++ -Wl,-Bstatic -lws2_32 -lpthread -lgomp -Wl,-Bdynamic)
endif
