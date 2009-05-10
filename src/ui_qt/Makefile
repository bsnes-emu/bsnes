objects := main $(if $(call streq,$(platform),win),resource) $(objects)

ifeq ($(moc),)
moc := moc
endif

ifeq ($(rcc),)
rcc := rcc
endif

ifeq ($(platform),x)
  qtflags = `pkg-config --cflags QtCore QtGui`
  link += `pkg-config --libs QtCore QtGui`
else ifeq ($(platform),win)
  ifeq ($(qtpath),)
    # find Qt install directory from PATH environment variable
    qtpath := $(foreach path,$(subst ;, ,$(PATH)),$(if $(wildcard $(path)/$(moc).exe),$(path)))
    qtpath := $(strip $(qtpath))
    qtpath := $(subst \,/,$(qtpath))
    qtpath := $(patsubst %/bin,%,$(qtpath))
  endif

  qtflags := -I$(qtpath)/include
  qtflags += -I$(qtpath)/include/QtCore
  qtflags += -I$(qtpath)/include/QtGui

  link += -L$(qtpath)/lib
  link += -L$(qtpath)/plugins/imageformats

  link += -lmingw32 -lqtmain -lQtGui -lcomdlg32 -loleaut32 -limm32 -lwinmm
  link += -lwinspool -lmsimg32 -lQtCore -lole32 -ladvapi32 -lws2_32 -luuid -lgdi32

  # optional image-file support:
  # link += -lqjpeg -lqmng
endif

moc_headers := $(call rwildcard,$(ui)/,%.mh)
moc_objects := $(patsubst %.mh,%.moc,$(moc_headers))

#############
### rules ###
#############

# automatically run moc on all .mh (.MocHeader) files
%.moc: $<; $(moc) -f $< -o $@
$(foreach f,$(moc_objects),$(eval $f: $(patsubst %.moc,%.mh,$f)))

obj/main.o: $(ui)/main.cpp $(call rwildcard,$(ui)/)
	$(call compile,$(qtflags))

$(ui)/resource/resource.rcc: $(ui)/resource/resource.qrc data/*
	$(rcc) $(ui)/resource/resource.qrc -o $(ui)/resource/resource.rcc

obj/resource.o: $(ui)/resource/resource.rc
	windres $(ui)/resource/resource.rc obj/resource.o

###############
### targets ###
###############

ui_build: $(ui)/resource/resource.rcc $(moc_objects);
ui_clean:
	-$(foreach f,$(moc_objects),@$(call delete,$f))
	-@$(call delete,$(ui)/resource/resource.rcc)
