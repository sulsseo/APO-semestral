CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++
#CC = gcc
#CXX = g++

CPPFLAGS = -I .
CFLAGS =-g -std=gnu99 -O1 -Wall
CXXFLAGS = -g -std=gnu++11 -O1 -Wall
LDFLAGS = -lrt -lpthread

SOURCES = main.cpp my_knobs.cpp my_painter.cpp font_rom8x16.c font_prop14x16.c mzapo_phys.c mzapo_parlcd.c
TARGET_EXE = app
TARGET_IP ?= 192.168.202.161
TARGET_DIR ?= /tmp/$(shell whoami)
TARGET_USER ?= root

OBJECTS += $(filter %.o,$(SOURCES:%.c=%.o))
OBJECTS += $(filter %.o,$(SOURCES:%.cpp=%.o))

#$(warning OBJECTS=$(OBJECTS))

ifeq ($(filter %.cpp,$(SOURCES)),)
LINKER = $(CC)
LDFLAGS += $(CFLAGS) $(CPPFLAGS)
else
LINKER = $(CXX)
LDFLAGS += $(CXXFLAGS) $(CPPFLAGS)
endif

%.o:%.c
	$(CC) $(CFLAGS) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) $(LDFLAGS) -L. $^ -o $@

.PHONY : dep all run

dep: depend

depend: $(SOURCES) *.h
	echo '# autogenerated dependencies' > depend
ifneq ($(filter %.c,$(SOURCES)),)
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M $(filter %.c,$(SOURCES)) \
	  >> depend
endif
ifneq ($(filter %.cpp,$(SOURCES)),)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -w -E -M $(filter %.cpp,$(SOURCES)) \
	  >> depend
endif

clean:
	rm -f *.o *.a $(OBJECTS) $(TARGET_EXE) depend

run: $(TARGET_EXE)
	ssh $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(TARGET_EXE) $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/$(TARGET_EXE)
	ssh -t $(TARGET_USER)@$(TARGET_IP) $(TARGET_DIR)/$(TARGET_EXE)

-include depend
