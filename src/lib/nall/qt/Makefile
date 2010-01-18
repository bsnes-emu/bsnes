# requires nall/Makefile

# imports:
# $(qtlibs) -- list of Qt components to link against

# exports the following symbols:
# $(moc)   -- meta-object compiler
# $(rcc)   -- resource compiler
# $(qtinc) -- includes for compiling
# $(qtlib) -- libraries for linking

ifeq ($(moc),)
moc := moc
endif

ifeq ($(rcc),)
rcc := rcc
endif

ifeq ($(platform),x)
  qtinc := `pkg-config --cflags $(qtlibs)`
  qtlib := `pkg-config --libs $(qtlibs)`
else ifeq ($(platform),osx)
  qtinc := $(foreach lib,$(qtlibs),-I/Library/Frameworks/$(lib).framework/Versions/4/Headers

  qtlib := -L/Library/Frameworks
  qtlib += $(foreach lib,$(qtlibs),-framework $(lib))
  qtlib += -framework Carbon
  qtlib += -framework Cocoa
  qtlib += -framework OpenGL
  qtlib += -framework AppKit
  qtlib += -framework ApplicationServices
else ifeq ($(platform),win)
  ifeq ($(qtpath),)
    # find Qt install directory from PATH environment variable
    qtpath := $(foreach path,$(subst ;, ,$(PATH)),$(if $(wildcard $(path)/$(moc).exe),$(path)))
    qtpath := $(strip $(qtpath))
    qtpath := $(subst \,/,$(qtpath))
    qtpath := $(patsubst %/bin,%,$(qtpath))
  endif

  qtinc := -I$(qtpath)/include
  qtinc += $(foreach lib,$(qtlibs),-I$(qtpath)/include/$(lib))

  qtlib := -L$(qtpath)/lib
  qtlib += -L$(qtpath)/plugins/imageformats

  qtlib += $(foreach lib,$(qtlibs),-l$(lib)4)
  qtlib += -lmingw32 -lqtmain -lcomdlg32 -loleaut32 -limm32 -lwinmm
  qtlib += -lwinspool -lmsimg32 -lole32 -ladvapi32 -lws2_32 -luuid -lgdi32
  qtlib += $(foreach lib,$(qtlibs),-l$(lib)4)

  # optional image-file support:
  # qtlib += -lqjpeg -lqmng
endif
