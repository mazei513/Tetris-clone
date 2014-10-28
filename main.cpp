#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "shapes.h"
#include <stdio.h>
#include <ctime>
#include <windows.h>
#include <cstdlib>
#include "graphics.h"

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

cgame::cgame()
{
    clear_board(gboard);

    clear_board(npboard);

    new_shape();
}

cgame::~cgame()
{

}

bool cgame::check_game_over()
{
    return game_over;
}

void cgame::clear_board(int board[GBOARD_HEIGHT+2][GBOARD_WIDTH+4])
{
    for(int i = 0; i < GBOARD_HEIGHT+2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH+4; j++)
        {
            board[i][j] = BEMPTY;
        }
    }
}

void cgame::print_piece2npboard(piece_info printed_piece)
{
    clear_board(npboard);

    int row = printed_piece.y;
    int col = printed_piece.x;

    for(int i = 0; i < SHAPESIZE; i++)
    {
        for(int j = 0; j < SHAPESIZE; j++)
        {
            npboard[row][col] = shapes[printed_piece.type][printed_piece.rot].shape[i][j];
            col++;
            if(col > (printed_piece.x + 3))
            {
                col = printed_piece.x;
            }
        }
        row++;
    }
}

void cgame::new_shape()
{
    int piece_type = rand() % TOTAL_SHAPES;
    int col = (GBOARD_WIDTH/2);
    int row = 0;

    cur_piece.type = piece_type;
    cur_piece.rot = 0;
    cur_piece.x = col;
    cur_piece.y = row;

    print_piece2npboard(cur_piece);

    if(!shape_valid(cur_piece))
    {
        game_over = true;
    }
}

void cgame::print_cur_piece_info()
{
    cout << cur_piece.type << " " << cur_piece.rot << " " << cur_piece.x << " " << cur_piece.y << endl;
}

void cgame::CW_rotate()
{
    piece_info temp_piece = cur_piece;
    piece_info initial_state = cur_piece;

    temp_piece.rot++;
    if(temp_piece.rot > 3)
    {
        temp_piece.rot -= 4;
    }
    clear_board(npboard);
    print_piece2npboard(temp_piece);

    if(!shape_valid(temp_piece))
    {
        if(temp_piece.type != 0)
        {
            shift_right();
            temp_piece.x++;

            if(!shape_valid(temp_piece))
            {
                shift_left();
                shift_left();
                temp_piece.x-=2;

                if(!shape_valid(temp_piece))
                {
                    shift_right();
                    shift_down();
                    temp_piece.x++;
                    temp_piece.y++;

                    if(!shape_valid(temp_piece))
                    {
                        shift_right();
                        temp_piece.x++;

                        if(!shape_valid(temp_piece))
                        {
                            shift_left();
                            shift_left();
                            temp_piece.x-=2;

                            if(!shape_valid(temp_piece))
                            {
                                shift_right();
                                shift_up();
                                temp_piece.x++;
                                temp_piece.y--;

                                clear_board(npboard);
                                print_piece2npboard(initial_state);
                            }
                            else
                            {
                                cur_piece = temp_piece;
                            }
                        }
                        else
                        {
                            cur_piece = temp_piece;
                        }
                    }
                    else
                    {
                        cur_piece = temp_piece;
                    }
                }
                else
                {
                    cur_piece = temp_piece;
                }
            }
            else
            {
                cur_piece = temp_piece;
            }
        }
        else
        {
            clear_board(npboard);
            print_piece2npboard(initial_state);
        }
    }
    else
    {
        cur_piece = temp_piece;
    }
}

void cgame::CCW_rotate()
{
    piece_info temp_piece = cur_piece;
    piece_info initial_state = cur_piece;

    temp_piece.rot+=3;
    if(temp_piece.rot > 3)
    {
        temp_piece.rot -= 4;
    }
    clear_board(npboard);
    print_piece2npboard(temp_piece);

    if(!shape_valid(temp_piece))
    {
        if(temp_piece.type == 0)
        {
            clear_board(npboard);
            print_piece2npboard(initial_state);
        }
        else
        {
            shift_left();
            temp_piece.x--;

            if(!shape_valid(temp_piece))
            {
                shift_right();
                shift_right();
                temp_piece.x+=2;

                if(!shape_valid(temp_piece))
                {
                    shift_left();
                    shift_down();
                    temp_piece.x--;
                    temp_piece.y++;

                    if(!shape_valid(temp_piece))
                    {
                        shift_left();
                        temp_piece.x--;

                        if(!shape_valid(temp_piece))
                        {
                            shift_right();
                            shift_right();
                            temp_piece.x+=2;

                            if(!shape_valid(temp_piece))
                            {
                                shift_left();
                                shift_up();
                                temp_piece.x--;
                                temp_piece.y--;

                                clear_board(npboard);
                                print_piece2npboard(initial_state);
                            }
                            else
                            {
                                cur_piece = temp_piece;
                            }
                        }
                        else
                        {
                            cur_piece = temp_piece;
                        }
                    }
                    else
                    {
                        cur_piece = temp_piece;
                    }
                }
                else
                {
                    cur_piece = temp_piece;
                }
            }
            else
            {
                cur_piece = temp_piece;
            }
        }
    }
    else
    {
        cur_piece = temp_piece;
    }
}

void cgame::lock_shape()
{
    for(int i = 0; i < GBOARD_HEIGHT + 2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH + 4; j++)
        {
            if(npboard[i][j] == BFILLED)
            {
                gboard[i][j] = npboard[i][j];
                npboard[i][j] = BEMPTY;
            }
        }
    }

    line_clear();

    new_shape();
}

void cgame::line_clear()
{
    for(int i = 0; i < GBOARD_HEIGHT; i++)
    {
        if((gboard[i][2]) &&
           (gboard[i][3]) &&
           (gboard[i][4]) &&
           (gboard[i][5]) &&
           (gboard[i][6]) &&
           (gboard[i][7]) &&
           (gboard[i][8]) &&
           (gboard[i][9]) &&
           (gboard[i][10]) &&
           (gboard[i][11]))
        {
            for(int k = i; k > 0; k--)
            {
                for(int j = 2; j < GBOARD_WIDTH + 2; j++)
                {
                    gboard[k][j] = gboard[k-1][j];
                }
            }
            for(int l = 2; l < GBOARD_WIDTH + 2; l++)
            {
                gboard[0][l] = 0;
            }
        }
    }
}

bool cgame::shape_valid(piece_info checked_piece)
{
    for(int i = 0; i < GBOARD_HEIGHT+2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH+4; j++)
        {
            if((gboard[i][j] == BFILLED) && (npboard[i][j] == BFILLED))
            {
                return false;
            }
        }
    }

    if((checked_piece.x + shapes[checked_piece.type][checked_piece.rot].xl_offset) < 2)
    {
        return false;
    }
    if((checked_piece.x + 3 - shapes[checked_piece.type][checked_piece.rot].xr_offset) >= GBOARD_WIDTH+2)
    {
        return false;
    }
    if((checked_piece.y + shapes[checked_piece.type][checked_piece.rot].yt_offset) < 0)
    {
        return false;
    }
    if((checked_piece.y + 3 - shapes[checked_piece.type][checked_piece.rot].yb_offset) >= GBOARD_HEIGHT)
    {
        return false;
    }

    return true;
}

void cgame::shape_down()
{
    cur_piece.y++;

    if(shape_valid(cur_piece))
    {
        shift_down();

        if(!shape_valid(cur_piece))
        {
            shift_up();
            cur_piece.y--;

            if(!lock)
            {
                lock_timer = timeGetTime();
                lock = true;
            }
            if(lock)
            {
                if((timeGetTime() - lock_timer) >= lock_delay)
                {
                    lock_shape();
                    lock = false;
                }
            }
        }
        else
        {
            lock = false;
        }
    }
    else
    {
        cur_piece.y--;

        if(!lock)
        {
            lock_timer = timeGetTime();
            lock = true;
        }
        if(lock)
        {
            if((timeGetTime() - lock_timer) >= lock_delay)
            {
                lock_shape();
                lock = false;
            }
        }
    }
}

void cgame::shift_down()
{
    for(int i = GBOARD_HEIGHT + 1; i > 0; i--)
    {
        for(int j = 0; j < GBOARD_WIDTH + 4; j++)
        {
            npboard[i][j] = npboard[i - 1][j];
        }
    }
    for(int k = 0; k < GBOARD_WIDTH + 4; k++)
    {
        npboard[0][k] = BEMPTY;
    }
}

void cgame::shape_left()
{
    cur_piece.x--;

    if(shape_valid(cur_piece))
    {
        shift_left();

        if(!shape_valid(cur_piece))
        {
            cur_piece.x++;
            shift_right();
        }
    }
    else
    {
        cur_piece.x++;
    }
}

void cgame::shift_left()
{
    for(int i = 0; i < GBOARD_HEIGHT + 2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH + 3; j++)
        {
            npboard[i][j] = npboard[i][j + 1];
        }
    }
    for(int k = 0; k < GBOARD_HEIGHT; k++)
    {
        npboard[k][GBOARD_WIDTH + 3] = BEMPTY;
    }
}

void cgame::shape_right()
{
    cur_piece.x++;

    if(shape_valid(cur_piece))
    {
        shift_right();

        if(!shape_valid(cur_piece))
        {
            cur_piece.x--;
            shift_left();
        }
    }
    else
    {
        cur_piece.x--;
    }
}

void cgame::shift_right()
{
    for(int i = 0; i < GBOARD_HEIGHT + 2; i++)
    {
        for(int j = GBOARD_WIDTH + 3; j > 0; j--)
        {
            npboard[i][j] = npboard[i][j - 1];
        }
    }
    for(int k = 0; k < GBOARD_HEIGHT + 2; k++)
    {
        npboard[k][0] = BEMPTY;
    }
}

void cgame::shape_up()
{
    cur_piece.y--;

    if(shape_valid(cur_piece))
    {
        shift_up();

        if(!shape_valid(cur_piece))
        {
            cur_piece.y++;
            shift_down();
        }
    }
    else
    {
        cur_piece.y++;
    }
}

void cgame::shift_up()
{
    for(int i = 0; i < GBOARD_HEIGHT + 1; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH + 4; j++)
        {
            npboard[i][j] = npboard[i + 1][j];
        }
    }
    for(int k = 0; k < GBOARD_WIDTH + 4; k++)
    {
        npboard[GBOARD_HEIGHT - 1][k] = BEMPTY;
    }
}

void cgame::hard_drop()
{
    while(shape_valid(cur_piece))
    {
        cur_piece.y++;

        if(shape_valid(cur_piece))
        {
            shift_down();

            if(!shape_valid(cur_piece))
            {
                shift_up();

                break;
            }
        }
        else
        {
            cur_piece.y--;

            break;
        }
    }

    lock_shape();
}

void cgame::print_npboard()
{
    for(int i = 0; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH; j++)
        {
            cout << npboard[i][j+2];
        }
        cout << endl;
    }
}

void cgame::print_gboard()
{
    int temp_board[GBOARD_HEIGHT+2][GBOARD_WIDTH+4];

    for(int i = 0; i < GBOARD_HEIGHT+2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH+4; j++)
        {
            temp_board[i][j] = gboard[i][j];

            if(npboard[i][j])
            {
                temp_board[i][j] = npboard[i][j];
            }
        }
    }

    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH; j++)
        {
            cout << temp_board[i][j+2];
        }
        cout << endl;
    }
}

void cgame::draw_pieces()
{
    blank_board();

    int temp_board[GBOARD_HEIGHT+2][GBOARD_WIDTH+4];

    for(int i = 0; i < GBOARD_HEIGHT+2; i++)
    {
        for(int j = 0; j < GBOARD_WIDTH+4; j++)
        {
            temp_board[i][j] = gboard[i][j];

            if(npboard[i][j])
            {
                temp_board[i][j] = npboard[i][j];
            }
        }
    }

    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            if(temp_board[i][j])
            {
                draw_solid_block(i, j);
            }
        }
    }

    piece_info initial_state;
    initial_state = cur_piece;

    while(shape_valid(cur_piece))
    {
        cur_piece.y++;

        if(shape_valid(cur_piece))
        {
            shift_down();

            if(!shape_valid(cur_piece))
            {
                shift_up();

                break;
            }
        }
        else
        {
            cur_piece.y--;

            break;
        }
    }

    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            if(npboard[i][j])
            {
                draw_ghost_block(i, j);
            }
        }
    }

    cur_piece = initial_state;
    clear_board(npboard);
    print_piece2npboard(cur_piece);
}

void cgame::restart()
{
    game_over = false;

    clear_board(gboard);
    clear_board(npboard);

    new_shape();

    init_board_graphics();
}

void cgame::handle_input(bool &quit)
{
    SDL_Event e;
    SDL_Keycode keypressed = 0;
    SDL_Keycode last_keypressed = 0;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if(check_game_over())
    {
        game_over_screen();

        while(!quit)
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }

            if(currentKeyStates[SDL_SCANCODE_RETURN])
            {
                restart();
                break;
            }
        }
    }

    current_time = timeGetTime();

    if((current_time - last_time) >= difficulty)
    {
        shape_down();
        //print_cur_piece_info();
        last_time = current_time;
        draw_pieces();
    }
    while( SDL_PollEvent( &e ) != 0 )
    {
        if( e.type == SDL_QUIT )
        {
            quit = true;
        }
    }

    if(currentKeyStates[SDL_SCANCODE_DOWN])
    {
        if((timeGetTime() - down_timer) >= down_time)
        {
            shape_down();
            //print_cur_piece_info();
            down_timer = timeGetTime();
            last_time = timeGetTime();
            draw_pieces();
        }
    }

    if(lr_pressed)
    {
        if(currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
        {
            if(lr_delay)
            {
                if((timeGetTime() - lr_timer) >= lr_delay_time)
                {
                    shape_right();
                    //print_cur_piece_info();
                    lr_timer = timeGetTime();
                    draw_pieces();
                    lr_delay = false;
                }
            }
            if(!lr_delay)
            {
                if((timeGetTime() - lr_timer) >= (lr_speed))
                {
                    shape_right();
                    //print_cur_piece_info();
                    lr_timer = timeGetTime();
                    draw_pieces();
                    lr_delay = false;
                }
            }
        }
        if(!currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
        {
            if(lr_delay)
            {
                if((timeGetTime() - lr_timer) >= lr_delay_time)
                {
                    shape_left();
                    //print_cur_piece_info();
                    lr_timer = timeGetTime();
                    draw_pieces();
                    lr_delay = false;
                }
            }
            if(!lr_delay)
            {
                if((timeGetTime() - lr_timer) >= (lr_speed))
                {
                    shape_left();
                    //print_cur_piece_info();
                    lr_timer = timeGetTime();
                    draw_pieces();
                    lr_delay = false;
                }
            }
        }
    }
    if(!lr_pressed)
    {
        if(currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
        {
            shape_right();
            draw_pieces();
            lr_timer = timeGetTime();
            lr_pressed = true;
        }

        if(currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_RIGHT])
        {
            shape_left();
            //print_cur_piece_info();
            lr_timer = timeGetTime();
            draw_pieces();
            lr_pressed = true;
        }
    }
    if(!currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
    {
        lr_pressed = false;
        lr_delay = true;
    }
    if(space_pressed)
    {
        if(!currentKeyStates[SDL_SCANCODE_SPACE])
        {
            space_pressed = false;
        }
    }
    if(!space_pressed)
    {
        if(currentKeyStates[SDL_SCANCODE_SPACE])
        {
            space_pressed = true;
            hard_drop();
            //print_cur_piece_info();
            last_time = timeGetTime();
            draw_pieces();
        }
    }
    if(rotate_pressed)
    {
        if(!currentKeyStates[SDL_SCANCODE_Z] && !currentKeyStates[SDL_SCANCODE_X])
        {
            rotate_pressed = false;
        }
    }
    if(!rotate_pressed)
    {
        if(currentKeyStates[SDL_SCANCODE_Z] && !currentKeyStates[SDL_SCANCODE_X])
        {
            rotate_pressed = true;
            CCW_rotate();
            //print_cur_piece_info();
            draw_pieces();
        }
        if(currentKeyStates[SDL_SCANCODE_X] && !currentKeyStates[SDL_SCANCODE_Z])
        {
            rotate_pressed = true;
            CW_rotate();
            //print_cur_piece_info();
            draw_pieces();
        }
    }
    if(currentKeyStates[SDL_SCANCODE_ESCAPE])
    {
        quit = true;
    }
}

int main(int argv, char* argc[])
{
    srand(time(NULL));

    cgame Game;

    bool quit = false;

    Game.init_board_graphics();

    while(!quit)
    {
        Game.handle_input(quit);

        Game.render_screen();
    }

    return 0;
}
