CC = g++
FILES = main.cpp shapes.h shapes.cpp graphics.h graphics.cpp gamegraphics.h gamegraphics.cpp game.h game.cpp
OUT_EXE = tetris-clone
INC_PATHS = -IC:\SDL\include\SDL2

LIB_PATHS = -LC:\SDL\lib

# -Wl,-subsystem,windows gets rid of console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lwinmm -lSDL2_image

all : $(FILES)
		$(CC) $(FILES) $(INC_PATHS) $(LIB_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUT_EXE)

