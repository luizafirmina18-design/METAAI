ifeq ($(strip $(PSL1GHT)),)
$(error "Defina a variavel PSL1GHT")
endif
include $(PSL1GHT)/ppu_rules

.DEFAULT_GOAL := all

TARGET    := game
TITLE     := slips3
APPID     := SLIP00001
CONTENTID := UP0001-$(APPID)_00-0000000000000000
SFOXML    := sfo.xml
PORTLIBS  := $(PS3DEV)/portlibs/ppu

MACHDEP  := -mhard-float -fmodulo-sched -ffunction-sections -fdata-sections
CXXFLAGS := -O2 -Wall -mcpu=cell $(MACHDEP) -Iinclude -I$(PSL1GHT)/ppu/include -I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL
LDFLAGS  := $(MACHDEP) -L$(PSL1GHT)/ppu/lib -L$(PORTLIBS)/lib
LIBS     := -lSDL -laudio -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lm

OBJS     := source/main.o source/game.o source/render.o

all: $(TARGET).pkg

source/%.o: source/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(TARGET).pkg: $(TARGET).elf $(SFOXML)
	rm -rf pkg
	mkdir -p pkg/USRDIR
	python3 -c 'import zlib,struct,math;W,H=320,176;buf=bytearray(W*H*3);[(buf.__setitem__(slice((y*W+x)*3,(y*W+x)*3+3),bytes((15,20,30) if (x%20==0 or y%20==0) else (10,14,22)))) for y in range(H) for x in range(W)];circ=lambda cx,cy,r,c:[(buf.__setitem__(slice((y*W+x)*3,(y*W+x)*3+3),bytes(c))) for y in range(max(0,int(cy-r)),min(H,int(cy+r+1))) for x in range(max(0,int(cx-r)),min(W,int(cx+r+1))) if (x-cx)**2+(y-cy)**2<=r*r];[circ(40+i*6,88+math.sin(i*0.25)*35,14,(0,180,70) if i%2 else (0,230,90)) for i in range(34)];circ(244,88+math.sin(34*0.25)*35,17,(0,240,100));circ(250,82+math.sin(34*0.25)*35,5,(255,255,255));circ(250,94+math.sin(34*0.25)*35,5,(255,255,255));circ(252,82+math.sin(34*0.25)*35,2,(0,0,0));circ(252,94+math.sin(34*0.25)*35,2,(0,0,0));circ(285,60,7,(255,60,80));circ(275,125,8,(255,210,0));circ(130,35,6,(0,190,255));raw=b"".join(b"\x00"+bytes(buf[y*W*3:(y+1)*W*3]) for y in range(H));chk=lambda t,d:struct.pack("!I",len(d))+t+d+struct.pack("!I",zlib.crc32(t+d)&0xffffffff);open("pkg/ICON0.PNG","wb").write(b"\x89PNG\r\n\x1a\n"+chk(b"IHDR",struct.pack("!IIBBBBB",W,H,8,2,0,0,0))+chk(b"IDAT",zlib.compress(raw))+chk(b"IEND",b""))'
	$(PS3DEV)/bin/sprxlinker $(TARGET).elf
	$(PS3DEV)/bin/make_self_npdrm $(TARGET).elf pkg/USRDIR/EBOOT.BIN $(CONTENTID)
	$(PS3DEV)/bin/sfo.py --title "$(TITLE)" --appid "$(APPID)" -f $(SFOXML) pkg/PARAM.SFO
	$(PS3DEV)/bin/pkg.py --contentid $(CONTENTID) pkg/ $@

clean:
	rm -rf pkg source/*.o *.elf *.self *.pkg
