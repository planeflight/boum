CC = emcc

BIN = build
APP = index.html

SRC = ${wildcard src/*.cpp}
OBJ = $(patsubst %.cpp, $(BIN)/%.o, $(SRC))


CFLAGS = -Wall -Wextra -Wcast-qual -Wconversion-null -Wformat-security
CFLAGS += -Wmissing-declarations -Woverlength-strings -Wpointer-arith -Wundef
CFLAGS += -Wunused-local-typedefs -Wunused-result -Wvarargs -Wvla
CFLAGS += -Wwrite-strings -DNOMINMAX -fno-omit-frame-pointer
CFLAGS += -std=c++20 -fPIC

ASSET_PATH = res/

LIB = -Llib/raylib/src/ lib/raylib/src/libraylib.web.a
INCLUDE = -Ilib/raylib/src/ -Ilib/raylib/src/external -Ilib/raylib/src/external/glfw/include
EMCC_FLAGS = -sUSE_GLFW=3  --shell-file res/web/shell-debug.html --preload-file $(ASSET_PATH)

DEFINES = -DPLATFORM_WEB

OPT = -O2


all: deps dirs $(BIN)/$(APP)

deps:
	cd lib/raylib/src && make PLATFORM=PLATFORM_WEB

dirs:
	mkdir -p $(BIN)

$(BIN)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE) $(OPT) $(DEFINES)

$(BIN)/$(APP): dirs $(OBJ)
	mkdir -p $(BIN)
	$(CC) -o $(BIN)/$(APP) $(OBJ) $(LIB) $(EMCC_FLAGS) $(DEFINES)

clean:
	rm -rf $(BIN)
