ifeq ($(strip $(PSL1GHT)),)
$(error "Defina a variavel PSL1GHT")
endif
include $(PSL1GHT)/ppu_rules

.DEFAULT_GOAL := all

TARGET    := slips3
TITLE     := slips3
APPID     := SLIP00001
CONTENTID := UP0001-$(APPID)_00-0000000000000000
PORTLIBS  := $(PS3DEV)/portlibs/ppu

MACHDEP := -mhard-float -fmodulo-sched -ffunction-sections -fdata-sections
CXXFLAGS := -O2 -Wall -mcpu=cell $(MACHDEP) -Iinclude -I$(PSL1GHT)/ppu/include -I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL
LDFLAGS  := $(MACHDEP) -L$(PSL1GHT)/ppu/lib -L$(PORTLIBS)/lib
LIBS     := -lSDL -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lm

SOURCES := $(wildcard source/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGET).pkg

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(TARGET).pkg: $(TARGET).elf
	rm -rf pkg
	mkdir -p pkg/USRDIR
	if [ -f ICON0.PNG ]; then cp ICON0.PNG pkg/ICON0.PNG; fi
	$(PS3DEV)/bin/sprxlinker $(TARGET).elf
	$(PS3DEV)/bin/make_self_npdrm $(TARGET).elf pkg/USRDIR/EBOOT.BIN $(CONTENTID)
	$(PS3DEV)/bin/sfo.py --title "$(TITLE)" --appid "$(APPID)" pkg/PARAM.SFO
	$(PS3DEV)/bin/pkg.py --contentid $(CONTENTID) pkg/ $@

clean:
	rm -rf pkg $(OBJECTS) *.elf *.self *.pkg