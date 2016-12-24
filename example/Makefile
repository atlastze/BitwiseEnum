# Makefile - do not edit!

# Include project file 
include $(wildcard *.pro)

# Search path for source and header files
VPATH = $(DEPENDPATH)
INCLUDE = $(addprefix -I,$(INCLUDEPATH))

# Objects
OBJECTS := $(subst .cpp,.o,$(notdir $(SOURCES)))
OBJECTS := $(subst .c,.o,$(notdir $(OBJECTS)))

# Build targets
$(TARGET): $(OBJECTS)
	$(LINK.cpp) -o $@ $^

%.o: %.cpp
	$(COMPILE.cpp) $(INCLUDE) -O2 -MMD -MP -MF .depends/$@.d -o $@ $<

%.o: %.c
	$(COMPILE.c) $(INCLUDE) -O2 -MMD -MP -MF .depends/$@.d -o $@ $<

# Clean targets
.PHONY: clean
clean:
	rm -rf *.o .depends $(TARGET)

# Create dependencies directory
$(shell [ ! -e .depends ] && mkdir .depends)

# Enable dependency checking
DEPENDS = $(wildcard .depends/*.d)
ifneq ($(DEPENDS),)
include $(DEPENDS)
endif
