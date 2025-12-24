# Flapwii-Bird - Makefile
#---------------------------------------------------------------------------------
# Clear the implicit built-in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
.SECONDARY:

#---------------------------------------------------------------------------------
# Check if DEVKITPPC is set up correctly
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# Project Configuration
#---------------------------------------------------------------------------------
TARGET              := boot
BUILD               := build
DATA                := assets/textures assets/fonts assets/sfx
SOURCES             := src
INCLUDES            := src
LIBOGC_INC          := $(DEVKITPRO)/libogc/include
LIBOGC_LIB          := $(DEVKITPRO)/libogc/lib/wii
PORTLIBS            := $(DEVKITPRO)/portlibs/ppc

#---------------------------------------------------------------------------------
# Third Party Library Configuration (GRRLIB)
#---------------------------------------------------------------------------------
TP_DIR          := third_party
GRRLIB_VER      := v4.6.1
GRRLIB_URL      := https://github.com/GRRLIB/GRRLIB/archive/refs/tags/$(GRRLIB_VER).tar.gz
GRRLIB_ROOT     := $(TP_DIR)/grrlib
GRRLIB_INTERNAL := $(GRRLIB_ROOT)/GRRLIB/GRRLIB
PNGU_DIR        := $(GRRLIB_ROOT)/GRRLIB/lib/pngu

#---------------------------------------------------------------------------------
# Compiler and tools
#---------------------------------------------------------------------------------
CC      = $(DEVKITPPC)/bin/powerpc-eabi-gcc
CXX     = $(DEVKITPPC)/bin/powerpc-eabi-g++
STRIP   = $(DEVKITPPC)/bin/powerpc-eabi-strip

#---------------------------------------------------------------------------------
# Options for code generation
#---------------------------------------------------------------------------------
CFLAGS      := -g -O3 -Wall -DGEKKO $(MACHDEP) $(INCLUDE)
CXXFLAGS    := $(CFLAGS) -Wno-register
LDFLAGS     := -g $(MACHDEP) -Wl,-Map,$(notdir $@).map -Wl,--section-start,.init=0x81000000

#---------------------------------------------------------------------------------
# Libraries to link with
#---------------------------------------------------------------------------------
LIBS        := -lfreetype -lbz2 -lpng -ljpeg -lz -lfat \
               -lwiiuse -lbte -lasnd -logc -lm -lvorbisidec -logg

#---------------------------------------------------------------------------------
# List of directories containing libraries
#---------------------------------------------------------------------------------
LIBDIRS     := $(PORTLIBS)

#=================================================================================
# BUILD LOGIC - Top level makefile
#=================================================================================
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT   := $(CURDIR)/$(TARGET)
export VPATH    := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                   $(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
                   $(CURDIR)/$(GRRLIB_INTERNAL) \
                   $(CURDIR)/$(PNGU_DIR)

export DEPSDIR  := $(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# Use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(wildcard $(CURDIR)/src/*.cpp)),)
    export LD := $(CC)
else
    export LD := $(CXX)
endif

#---------------------------------------------------------------------------------
# Build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE  := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include/freetype2) \
                   -I$(LIBOGC_INC) \
                   -I$(CURDIR)/$(GRRLIB_INTERNAL) \
                   -I$(CURDIR)/$(PNGU_DIR) \
                   -I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# Build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                   -L$(LIBOGC_LIB)

.PHONY: $(BUILD) clean distclean all run download_grrlib

all: download_grrlib $(BUILD)

download_grrlib:
	@if [ ! -f "$(GRRLIB_INTERNAL)/grrlib.h" ]; then \
		echo "Downloading GRRLIB $(GRRLIB_VER)..."; \
		rm -rf $(GRRLIB_ROOT); \
		mkdir -p $(GRRLIB_ROOT); \
		curl -L $(GRRLIB_URL) -o grrlib.tar.gz; \
		tar -xzf grrlib.tar.gz -C $(GRRLIB_ROOT) --strip-components=1; \
		rm grrlib.tar.gz; \
		echo "GRRLIB ready."; \
	fi

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo "Cleaning project files..."
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(OUTPUT).map

distclean: clean
	@echo "Cleaning third_party libraries..."
	@rm -rf $(TP_DIR)

run:
	wiiload $(TARGET).dol

#=================================================================================
# BUILD LOGIC - Recursive makefile (inside BUILD)
#=================================================================================
else
#---------------------------------------------------------------------------------
# Discover source files now that they exist
GRRLIB_C    := $(notdir $(wildcard ../$(GRRLIB_INTERNAL)/*.c))
PNGU_C      := $(notdir $(wildcard ../$(PNGU_DIR)/*.c))
CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard ../$(dir)/*.c))) $(GRRLIB_C) $(PNGU_C)
CPPFILES    := $(foreach dir,$(SOURCES),$(notdir $(wildcard ../$(dir)/*.cpp)))
sFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard ../$(dir)/*.s)))
SFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard ../$(dir)/*.S)))
BINFILES    := $(foreach dir,$(DATA),$(notdir $(wildcard ../$(dir)/*.*)))

OFILES_BIN := $(addsuffix .o,$(BINFILES))
OFILES_SRC := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
OFILES     := $(OFILES_BIN) $(OFILES_SRC)

DEPENDS :=  $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# Main Targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)
	@echo "Linking ELF file..."
	@$(LD) -o $@ $(OFILES) $(LIBPATHS) $(LDFLAGS) $(LIBS)
	@echo "Stripping ELF file..."
	@$(STRIP) $@

#---------------------------------------------------------------------------------
# Binary Data Rules
#---------------------------------------------------------------------------------
%.png.o : %.png
	@echo $(notdir $<)
	@$(bin2o)

%.ttf.o : %.ttf
	@echo $(notdir $<)
	@$(bin2o)

%.ogg.o : %.ogg
	@echo $(notdir $<)
	@$(bin2o)

%.wav.o : %.wav
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

# EOF
