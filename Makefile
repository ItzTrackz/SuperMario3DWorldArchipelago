.SUFFIXES:

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

include $(DEVKITPRO)/wups/share/wups_rules

WUT_ROOT := $(DEVKITPRO)/wut
WUPS_ROOT := $(DEVKITPRO)/wups
WUMS_ROOT := $(DEVKITPRO)/wums

TARGET := SM3DWPlugin
BUILD := build

SOURCES := source source/sm3dw source/config
DATA := data
INCLUDES := source

CFLAGS := -Wall -Werror -O2 -ffunction-sections $(MACHDEP)
CFLAGS += $(INCLUDE) -D__WIIU__ -D__WUT__ -D__WUPS__

CXXFLAGS := $(CFLAGS) -std=gnu++20
ASFLAGS := -g $(ARCH)

LDFLAGS = -g $(ARCH) $(RPXSPECS) -Wl,-Map,$(notdir $*.map) $(WUPSSPECS)

ifeq ($(DEBUG),1)
CXXFLAGS += -DDEBUG -g
CFLAGS += -DDEBUG -g
endif

ifeq ($(DEBUG),VERBOSE)
CXXFLAGS += -DDEBUG -DVERBOSE_DEBUG -g
CFLAGS += -DDEBUG -DVERBOSE_DEBUG -g
endif

LIBS := -lwups -lwut -lnotifications

LIBDIRS := $(PORTLIBS) $(WUPS_ROOT) $(WUT_ROOT) $(WUT_ROOT)/usr $(WUMS_ROOT)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export TOPDIR := $(CURDIR)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifeq ($(strip $(CPPFILES)),)
export LD := $(CC)
else
export LD := $(CXX)
endif

export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_SRC := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN := $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                  -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all $(BUILD) clean

all: $(BUILD)

$(BUILD):
	@mkdir -p $(BUILD)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).wps $(TARGET).elf

else

DEPENDS := $(OFILES:.o=.d)

.PHONY: all

all: $(OUTPUT).wps

$(OUTPUT).wps: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES)
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

$(OFILES_SRC): $(HFILES_BIN)

%.bin.o %_bin.h: %.bin
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

endif
