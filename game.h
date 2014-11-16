#ifndef GAME_HEADER
#define GAME_HEADER

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "shapes.h"
#include <stdio.h>
#include <ctime>
#include <windows.h>
#include <cstdlib>
#include "gamegraphics.h"

using std::cout;
using std::endl;

enum BLOCK_STATE{
BEMPTY = 0,
BFILLED
};

struct piece_info
{
    int type;
    int rot;
    int x;
    int y;
};

extern PSHAPE shapes[TOTAL_SHAPES][4];

class cgame : public Graphics
{
private:
    int gboard[GBOARD_HEIGHT+2][GBOARD_WIDTH+4];
    int npboard[GBOARD_HEIGHT+2][GBOARD_WIDTH+4];
    piece_info cur_piece;
    void shift_down();
    void shift_left();
    void shift_right();
    void shift_up();
    void clear_board(int board[GBOARD_HEIGHT+2][GBOARD_WIDTH+4]);
    void print_piece2npboard(piece_info printed_piece);
    void line_clear();
    bool start_game = true;
    bool game_over = false;
    DWORD current_time = 0, last_time = 0, down_timer = 0, lr_timer = 0, lock_timer = 0;
    const DWORD down_time = 35, lr_delay_time = 150, difficulty = 1000, lock_delay = 500, lr_speed = 35;
    bool space_pressed = false, lr_pressed = false, rotate_pressed = false, lock = false, lr_delay = true;
public:
    cgame();
    ~cgame();
    void new_shape();
    void CW_rotate();
    void CCW_rotate();
    void lock_shape();
    bool shape_valid(piece_info checked_piece);
    void shape_down();
    void shape_left();
    void shape_right();
    void shape_up();
    void hard_drop();
    void print_npboard();           //debugging function
    void print_gboard();            //debugging function
    void print_cur_piece_info();    //debugging function
    bool check_game_over();
    void draw_pieces();
    void restart();
    void handle_input(bool &quit);
};

#endif
