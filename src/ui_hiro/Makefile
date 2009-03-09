##############################
### platform configuration ###
##############################

objects := main hiro $(if $(call streq,$(platform),win),resource) $(objects)

ifeq ($(platform),x)
  link += `pkg-config --libs gtk+-2.0`
  link += $(call mklib,Xtst)
  hiroflags = `pkg-config --cflags gtk+-2.0`
else ifeq ($(platform),win)
  link += $(call mklib,comctl32)
  link += $(call mklib,comdlg32)
  hiroflags =
endif

#############
### rules ###
#############

obj/main.$(obj): $(ui)/main.cpp $(ui)/* $(ui)/base/* $(ui)/loader/* $(ui)/settings/* $(ui)/event/*
obj/hiro.$(obj): lib/hiro/hiro.cpp lib/hiro/*
	$(call compile,$(hiroflags))
obj/resource.$(obj): $(ui)/bsnes.rc; windres $(ui)/bsnes.rc obj/resource.$(obj)

###############
### targets ###
###############

ui_build:;
ui_clean:;
