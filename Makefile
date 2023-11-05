
.PHONY: all clean
.DEFAULT: opt

SRCDIR := $(CURDIR)

FILES := \
  array.c \
  hash_map.c \
  map.c \
  sorted_array.c \
  main.c

HEADERS := \
  array.h \
  hash_map.h \
  map.h \
  sorted_array.h \
  geometry.h \
  item.h \
  timer.h

#sanitize:=-fsanitize=address
sanitize:=#
optimize:=-O3
#optimize:=-g

all: test

test: list_runner
	@./$< 10000

list_runner: $(FILES) $(HEADERS) Makefile
	@clang -o $@ $(optimize) $(sanitize) $(FILES)
