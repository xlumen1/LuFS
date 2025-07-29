CC = gcc
CFLAGS = -Wall -O2 -fPIC -g
AR = ar
ARFLAGS = rcs

LIB_STATIC = out/liblufs.a
LIB_SHARED = out/liblufs.so

SRC = src/lufs.c
OBJ = $(patsubst src/%.c,build/%.o,$(SRC))

all: $(LIB_STATIC) $(LIB_SHARED)

$(LIB_STATIC): $(OBJ)
	@mkdir -p out
	$(AR) $(ARFLAGS) $@ $^

$(LIB_SHARED): $(OBJ)
	@mkdir -p out
	$(CC) -shared -o $@ $^

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -rf out
