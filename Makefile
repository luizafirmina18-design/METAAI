# Makefile - Slither PS3 Homebrew
# Compativel com PSL1GHT + ps3dev

TITLE		:= Slither PS3
APPID		:= SLITHER001
CONTENTID	:= UP0001-SLITHER001_00-0000000000000000

# Pastas
INCLUDE		:= include
SOURCE		:= source
BUILD		:= build

# Arquivos do jogo
CXXSOURCES	:= source/main.cpp source/game.cpp source/render.cpp
INCLUDES	:= -I$(INCLUDE) -I$(PSL1GHT)/ppu/include -I$(PSL1GHT)/portlibs/ppu/include/SDL

# Flags
CXXFLAGS	:= -O2 -Wall -std=gnu++11 -mcpu=cell -mno-vrsave $(INCLUDES)
LDFLAGS		:= -L$(PSL1GHT)/ppu/lib -L$(PSL1GHT)/portlibs/ppu/lib

# Bibliotecas que voce ja usava
LIBS		:= -lSDL -lio -lsysutil -lsysmodule -lGCM_sys -lreality -lm -lstdc++ -lsysfs -lnet -lpsl1ght

# Nome do executavel
TARGET		:= $(BUILD)/slither_ps3
ELF		:= $(TARGET).elf
SELF		:= $(TARGET).self

# Regras do PSL1GHT
include $(PSL1GHT)/ppu_rules

# Compila todos os .cpp para .o
OBJS		:= $(CXXSOURCES:.cpp=.o)

all: $(SELF)

$(SELF): $(ELF)
	$(MAKE_FSELF) $(ELF) $(SELF)

$(ELF): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(ELF) $(SELF) $(BUILD)/*.pkg

# ESSA ERA A PARTE QUE FALTAVA - CRIA O .PKG
pkg: $(SELF)
	@mkdir -p $(BUILD)/pkg/USRDIR
	@cp $(SELF) $(BUILD)/pkg/USRDIR/EBOOT.BIN
	@echo "$(TITLE)" > $(BUILD)/pkg/PARAM.SFO.txt
	$(SFO) --title "$(TITLE)" --appid "$(APPID)" $(BUILD)/pkg/PARAM.SFO
	@cp $(PSL1GHT)/ppu/lib/ICON0.PNG $(BUILD)/pkg/ICON0.PNG 2>/dev/null || echo "sem icone, ok"
	$(PKG) --contentid $(CONTENTID) $(BUILD)/pkg/ $(BUILD)/$(APPID).pkg
	@echo "PKG GERADO: $(BUILD)/$(APPID).pkg"

.PHONY: all clean pkg