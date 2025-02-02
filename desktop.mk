CC = clang++

BIN = build
APP = app

SRC = ${wildcard src/*.cpp}
OBJ = $(patsubst %.cpp, $(BIN)/%.o, $(SRC))


CFLAGS = -Wall -Wextra -Wcast-qual -Wconversion-null -Wformat-security
CFLAGS += -Wmissing-declarations -Woverlength-strings -Wpointer-arith -Wundef
CFLAGS += -Wunused-local-typedefs -Wunused-result -Wvarargs -Wvla
CFLAGS += -Wwrite-strings -DNOMINMAX -fno-omit-frame-pointer
CFLAGS += -std=c++20 -fPIC

LIB = -Llib/raylib/src/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic
INCLUDE = -Ilib/raylib/src/ -Ilib/raylib/src/external -Ilib/raylib/src/external/glfw/include

DEFINES = -DPLATFORM_DESKTOP -DPLATFORM_DESKTOP_GLFW

OPT = -O2


all: deps dirs $(BIN)/$(APP)

deps:
	cd lib/raylib/src && make PLATFORM=PLATFORM_DESKTOP

dirs:
	mkdir -p $(BIN)

$(BIN)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE) $(OPT) $(DEFINES)

$(BIN)/$(APP): dirs $(OBJ)
	mkdir -p $(BIN)
	$(CC) -o $(BIN)/$(APP) $(OBJ) $(LIB)

clean:
	rm -r $(BIN)
