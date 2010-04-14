include nall/Makefile

qtlibs := QtCore QtGui
include nall/qt/Makefile

c     := $(compiler) -std=gnu99
cpp   := $(subst cc,++,$(compiler)) -std=gnu++0x
flags := -O3 -I. -Iobj -fomit-frame-pointer
link  :=

ifeq ($(platform),x)
  flags := -fPIC $(flags)
  link += -s
else ifeq ($(platform),osx)
  flags := -fPIC $(flags)
endif

objects := snesreader

# fex
objects += Binary_Extractor blargg_common blargg_errors Data_Reader fex File_Extractor Gzip_Extractor Gzip_Reader Rar_Extractor Zip7_Extractor Zip_Extractor Zlib_Inflater
# zlib
objects += adler32 crc32 inffast inflate inftrees zutil
# 7-zip
objects += 7zAlloc 7zBuf 7zCrc 7zDecode 7zExtract 7zHeader 7zIn 7zItem 7zStream Bcj2 Bra86 LzmaDec
# unrar
objects += archive arcread coder crc encname extract getbits model rarvm rarvmtbl rawread suballoc unicode unpack unpack15 unpack20 unrar unrar_misc unrar_open
# micro-bunzip
objects += micro-bunzip
# jma
objects += jma jcrc32 lzmadecode 7zlzma iiostrm inbyte lzma winout

compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(cpp) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

objects := $(patsubst %,obj/%.o,$(objects))
moc_headers := $(call rwildcard,./,%.moc.hpp)
moc_objects := $(foreach f,$(moc_headers),obj/$(notdir $(patsubst %.moc.hpp,%.moc,$f)))

# automatically run moc on all .moc.hpp (MOC header) files
%.moc: $<; $(moc) -i $< -o $@

# automatically generate %.moc build rules
__list = $(moc_headers)
$(foreach f,$(moc_objects), \
  $(eval __file = $(word 1,$(__list))) \
  $(eval __list = $(wordlist 2,$(words $(__list)),$(__list))) \
  $(eval $f: $(__file)) \
)

##################
### snesreader ###
##################

obj/snesreader.o: snesreader.cpp *
	$(call compile,$(qtinc))

###########
### fex ###
###########

obj/Binary_Extractor.o: fex/Binary_Extractor.cpp fex/*
obj/blargg_common.o   : fex/blargg_common.cpp fex/*
obj/blargg_errors.o   : fex/blargg_errors.cpp fex/*
obj/Data_Reader.o     : fex/Data_Reader.cpp fex/*
obj/fex.o             : fex/fex.cpp fex/*
obj/File_Extractor.o  : fex/File_Extractor.cpp fex/*
obj/Gzip_Extractor.o  : fex/Gzip_Extractor.cpp fex/*
obj/Gzip_Reader.o     : fex/Gzip_Reader.cpp fex/*
obj/Rar_Extractor.o   : fex/Rar_Extractor.cpp fex/*
obj/Zip7_Extractor.o  : fex/Zip7_Extractor.cpp fex/*
obj/Zip_Extractor.o   : fex/Zip_Extractor.cpp fex/*
obj/Zlib_Inflater.o   : fex/Zlib_Inflater.cpp fex/*

############
### zlib ###
############

obj/adler32.o : zlib/adler32.c zlib/*
obj/crc32.o   : zlib/crc32.c zlib/*
obj/inffast.o : zlib/inffast.c zlib/*
obj/inflate.o : zlib/inflate.c zlib/*
obj/inftrees.o: zlib/inftrees.c zlib/*
obj/zutil.o   : zlib/zutil.c zlib/*

#############
### 7-zip ###
#############

obj/7zAlloc.o  : 7z_C/7zAlloc.c 7z_C/*
obj/7zBuf.o    : 7z_C/7zBuf.c 7z_C/*
obj/7zCrc.o    : 7z_C/7zCrc.c 7z_C/*
obj/7zDecode.o : 7z_C/7zDecode.c 7z_C/*
obj/7zExtract.o: 7z_C/7zExtract.c 7z_C/*
obj/7zHeader.o : 7z_C/7zHeader.c 7z_C/*
obj/7zIn.o     : 7z_C/7zIn.c 7z_C/*
obj/7zItem.o   : 7z_C/7zItem.c 7z_C/*
obj/7zStream.o : 7z_C/7zStream.c 7z_C/*
obj/Bcj2.o     : 7z_C/Bcj2.c 7z_C/*
obj/Bra86.o    : 7z_C/Bra86.c 7z_C/*
obj/LzmaDec.o  : 7z_C/LzmaDec.c 7z_C/*

####################
### micro-bunzip ###
####################

obj/micro-bunzip.o: micro-bunzip/micro-bunzip.c micro-bunzip/*

#############
### unrar ###
#############

obj/archive.o   : unrar/archive.cpp unrar/*
obj/arcread.o   : unrar/arcread.cpp unrar/*
obj/coder.o     : unrar/coder.cpp unrar/*
obj/crc.o       : unrar/crc.cpp unrar/*
obj/encname.o   : unrar/encname.cpp unrar/*
obj/extract.o   : unrar/extract.cpp unrar/*
obj/getbits.o   : unrar/getbits.cpp unrar/*
obj/model.o     : unrar/model.cpp unrar/*
obj/rarvm.o     : unrar/rarvm.cpp unrar/*
obj/rarvmtbl.o  : unrar/rarvmtbl.cpp unrar/*
obj/rawread.o   : unrar/rawread.cpp unrar/*
obj/suballoc.o  : unrar/suballoc.cpp unrar/*
obj/unicode.o   : unrar/unicode.cpp unrar/*
obj/unpack.o    : unrar/unpack.cpp unrar/*
obj/unpack15.o  : unrar/unpack15.cpp unrar/*
obj/unpack20.o  : unrar/unpack20.cpp unrar/*
obj/unrar.o     : unrar/unrar.cpp unrar/*
obj/unrar_misc.o: unrar/unrar_misc.cpp unrar/*
obj/unrar_open.o: unrar/unrar_open.cpp unrar/*

##############
### libjma ###
##############

obj/jma.o       : libjma/jma.cpp libjma/*
obj/jcrc32.o    : libjma/jcrc32.cpp libjma/*
obj/lzmadecode.o: libjma/lzmadecode.cpp libjma/*
obj/7zlzma.o    : libjma/7zlzma.cpp libjma/*
obj/iiostrm.o   : libjma/iiostrm.cpp libjma/*
obj/inbyte.o    : libjma/inbyte.cpp libjma/*
obj/lzma.o      : libjma/lzma.cpp libjma/*
obj/winout.o    : libjma/winout.cpp libjma/*

###############
### targets ###
###############

build: $(moc_objects) $(objects)
ifeq ($(platform),x)
	ar rcs libsnesreader.a $(objects)
	$(cpp) $(link) -o libsnesreader.so -shared -Wl,-soname,libsnesreader.so.1 $(objects) $(qtlib)
else ifeq ($(platform),osx)
	ar rcs libsnesreader.a $(objects)
	$(cpp) $(link) -o libsnesreader.dylib -shared -dynamiclib $(objects) $(qtlib)
else ifeq ($(platform),win)
	$(cpp) $(link) -o snesreader.dll -shared -Wl,--out-implib,libsnesreader.a $(objects) $(qtlib)
endif

install:
ifeq ($(platform),x)
	install -D -m 755 libsnesreader.a $(DESTDIR)$(prefix)/lib
	install -D -m 755 libsnesreader.so $(DESTDIR)$(prefix)/lib
	ldconfig -n $(DESTDIR)$(prefix)/lib
else ifeq ($(platform),osx)
	cp libsnesreader.dylib /usr/local/lib/libsnesreader.dylib
endif

clean:
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.moc)
	-@$(call delete,libsnesreader.a)
	-@$(call delete,libsnesreader.so)
	-@$(call delete,libsnesreader.dylib)
	-@$(call delete,snesreader.dll)
