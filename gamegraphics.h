#ifndef GAMEGRAPHICS_HEADER
#define GAMEGRAPHICS_HEADER

#define GHOSTRGB 0xFF
#define GRIDRGB 0x40
#define GHOSTALPHA 0x80
#define GRIDALPHA 0x80

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "graphics.h"

const int BOARD_HEIGHT_OFFSET = 50, BOARD_WIDTH_OFFSET = 25, BOARD_HEIGHT_OFFSET_SIZE = 100, BOARD_WIDTH_OFFSET_SIZE = 50;
const int SCREEN_WIDTH = 350;
const int SCREEN_HEIGHT = 700;
const int GBOARD_WIDTH = 10;
const int GBOARD_HEIGHT = 22; //rows 0 and 1 is for new pieces to spawn into

class Graphics : public SDLGraphics
{
private:
    TTF_Font* gfont = NULL;
    SDL_Rect blockRect = {0, 0, ((SCREEN_WIDTH-BOARD_WIDTH_OFFSET_SIZE)/10), ((SCREEN_HEIGHT-BOARD_HEIGHT_OFFSET_SIZE)/20)};
    SDL_Rect boardRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect screen_size = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect boardSize = {BOARD_WIDTH_OFFSET, BOARD_HEIGHT_OFFSET, SCREEN_WIDTH - BOARD_WIDTH_OFFSET_SIZE, SCREEN_HEIGHT - BOARD_HEIGHT_OFFSET_SIZE};
public:
    Graphics();
    ~Graphics();
    void init_board_graphics();
    void draw_solid_block(int i, int j);
    void draw_ghost_block(int i, int j);
    void blank_board();
    void game_over_screen();
};

#endif
