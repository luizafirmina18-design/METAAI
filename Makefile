ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

include $(PSL1GHT)/ppu_rules

TARGET    := slips3
BUILD     := build
SOURCES   := source
DATA      := data
INCLUDES  := include

TITLE     := slips3
APPID     := SLIP00001
CONTENTID := UP0001-$(APPID)_00-0000000000000000

CFLAGS    := -O2 -Wall -mcpu=cell $(MACHDEP) $(INCLUDE)
CXXFLAGS  := $(CFLAGS)
LDFLAGS   := $(MACHDEP) -Wl,-Map,$(notdir $@).map

LIBS      := -lSDL -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lm

#---------------------------------------------------------------------------------
# Não altere abaixo desta linha a menos que saiba o que está fazendo
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT   := $(CURDIR)/$(TARGET)
export VPATH    := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                   $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR  := $(CURDIR)/$(BUILD)

CFILES          := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES        := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES          := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES          := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES        := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export LD       := $(CXX)
export OFILES   := $(addsuffix .o,$(BINFILES)) \
                   $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
                   $(sFILES:.s=.o) $(SFILES:.S=.o)

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(CURDIR)/$(BUILD) \
                   -I$(PSL1GHT)/ppu/include \
                   -I$(PS3DEV)/portlibs/ppu/include \
                   -I$(PS3DEV)/portlibs/ppu/include/SDL

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                   -L$(PSL1GHT)/ppu/lib \
                   -L$(PS3DEV)/portlibs/ppu/lib

.PHONY: $(BUILD) clean pkg

all: $(BUILD) pkg

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

pkg: $(BUILD)
	@rm -rf pkg
	@mkdir -p pkg/USRDIR
	@if [ -f ICON0.PNG ]; then cp ICON0.PNG pkg/ICON0.PNG; fi
	@$(PS3DEV)/bin/sprxlinker $(TARGET).elf
	@$(PS3DEV)/bin/make_self_npdrm $(TARGET).elf pkg/USRDIR/EBOOT.BIN $(CONTENTID)
	@$(PS3DEV)/bin/sfo.py --title "$(TITLE)" --appid "$(APPID)" pkg/PARAM.SFO
	@$(PS3DEV)/bin/pkg.py --contentid $(CONTENTID) pkg/ $(TARGET).pkg

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).self $(TARGET).pkg pkg

else

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif
