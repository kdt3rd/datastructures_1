
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

sanitize:=-fsanitize=address
#sanitize:=#

all: test

test: list_runner
	@./$<

list_runner: $(FILES) $(HEADERS)
	@clang -o $@ -g $(sanitize) $(FILES)
