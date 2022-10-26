CC      = g++
CFLAGS  = -std=c++20 -Wall -Wextra -Wpedantic -fdiagnostics-color=always
LFLAGS  =
TARGETS = generator maze solver
MODULES = grid util
OBJECTS = $(patsubst %,%.o,$(MODULES))
TOBJECTS= $(patsubst %,%.o,$(TARGETS))      # target objects

.PHONY: all debug clean tidy test

all: CFLAGS += -O3 -s
all: $(TARGETS)

debug: CFLAGS += -g
debug: $(TARGETS)

$(TARGETS): %: %.o $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.cpp %.h
	$(CC) -c $< $(CFLAGS)

$(TOBJECTS): %.o: %.cpp
	$(CC) -c $< $(CFLAGS)

test: all
	@ ./.scripts/test.sh

clean: tidy
	$(RM) $(TARGETS)
	$(RM) asgn3.tar.gz

tidy:
	$(RM) $(OBJECTS) $(patsubst %,%.o,$(TARGETS))

submit: clean
	tar --exclude='.vs' --exclude='*.gch' --exclude='*.tar.gz' --exclude=examples \
	    --exclude=.git --exclude=.gitignore -cvzf asgn3.tar.gz .
