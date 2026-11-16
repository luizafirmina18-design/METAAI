# Slither PS3 - Makefile oficial PSL1GHT
TITLE		:= Slither PS3
APPID		:= SLITHER001
CONTENTID	:= UP0001-SLITHER001_00-0000000000000000

TARGET		:= slither_ps3
BUILD		:= build
SOURCES		:= source
INCLUDES	:= include

# Arquivos C++
CXXFILES	:= $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))

# Flags
CFLAGS		:= -O2 -Wall -mcpu=cell
CXXFLAGS	:= $(CFLAGS) -std=gnu++11
INCLUDE		:= $(foreach dir,$(INCLUDES),-I$(dir)) \
		   -I$(PSL1GHT)/ppu/include \
		   -I$(PSL1GHT)/portlibs/ppu/include \
		   -I$(PSL1GHT)/portlibs/ppu/include/SDL

LIBDIRS		:= -L$(PSL1GHT)/ppu/lib -L$(PSL1GHT)/portlibs/ppu/lib
LIBS		:= -lSDL -lio -lsysutil -lsysmodule -lGCM_sys -lreality -lm -lstdc++ -lsysfs -lnet

# Isso aqui que cria o .pkg sozinho, nao precisa inventar regra
include $(PSL1GHT)/ppu_rules