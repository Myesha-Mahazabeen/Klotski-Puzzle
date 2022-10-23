SOURCES := $(wildcard *.c src/**/*.c *.cpp src/**/*.cpp)
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
HEADERS := $(wildcard *.h include/*.h)

COMMON   := -O2 -Wall -Wformat=2 -march=native -DNDEBUG
CFLAGS   := $(CFLAGS) $(COMMON) -D_REENTRANT
CXXFLAGS := $(CXXFLAGS) $(COMMON) -D_REENTRANT
CC       := gcc
CXX      := g++
LD       := $(CXX)
LDFLAGS  := $(LDFLAGS)  # -L/path/to/libs/
LDADD    :=  -lSDL2 # -lSDL2_ttf
INCLUDE  :=  # -I../path/to/headers/
DEFS     :=  # -DLINUX

TARGETS  := solver

.PHONY : all
all : $(TARGETS)

# {{{ for debugging
# DBGFLAGS := -g -UNDEBUG -O0
DBGFLAGS := -g -UNDEBUG
debug : CFLAGS += $(DBGFLAGS)
debug : CXXFLAGS += $(DBGFLAGS)
debug : all
.PHONY : debug
# }}}

$(TARGETS) : % : %.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDADD)

%.o : %.cpp $(HEADERS)
	$(CXX) $(DEFS) $(INCLUDE) $(CXXFLAGS) -c $< -o $@

%.o : %.c $(HEADERS)
	$(CC) $(DEFS) $(INCLUDE) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean :
	rm -f $(TARGETS) $(OBJECTS)

# vim:ft=make:foldmethod=marker:foldmarker={{{,}}}

