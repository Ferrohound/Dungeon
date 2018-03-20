#==============================================================================

#it's been way too long...

#==============================================================================
.PHONY: all clean


CC = g++
CFLAGS = -std=c++11
INCLUDES = -I /src
LIBS = -L
EXT ?= .exe

PLATFORM ?= PLATFORM_WINDOWS

ifeq ($(PLATFORM), PLATFORM_WINDOWS)
	EXT = .exe
endif

SOURCE = \
	src/world.cpp \ 

OBJS = $(patsubst %.cpp, %.o, $(SOURCE))

# typing 'make' will invoke the default target entry
default: main

# compile program
main: main.cpp $(OBJS)
	$(CC) -o /build/$@$(EXT) $^ $(CFLAGS) $(INCLUDES) $(CFLAGS) $(LIBS)

	
# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES) -$(LIBS)

clean: 
	del *.o *.exe /s