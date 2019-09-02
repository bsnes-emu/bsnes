name := libretro.so
flags += -std=c++17 -fpermissive -Wno-narrowing -Wno-multichar -fopenmp -g
#flags += -DSFC_SUPERGAMEBOY 

#include fc/GNUmakefile
#include sfc/GNUmakefile
#include ms/GNUmakefile
#include md/GNUmakefile
#include pce/GNUmakefile
#include gb/GNUmakefile
#include gba/GNUmakefile
#include ws/GNUmakefile
#include processor/GNUmakefile

# rules
objects := libretro $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/libretro.o: target-libretro/libretro.cpp

# targets
build: $(objects)
ifeq ($(platform),linux)
	$(strip $(compiler) -o out/bsnes_libretro.so -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -Wl,-Bdynamic -lpthread -ldl -lgomp)
else ifeq ($(platform),windows)
	$(strip $(compiler) -o out/bsnes_libretro.dll -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -static-libgcc -static-libstdc++ -Wl,-Bstatic -lws2_32 -lpthread -lgomp -Wl,-Bdynamic)
endif
