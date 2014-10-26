//to do
//one day try to code in srs wall kicks
//hold piece
//piece queue

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "shapes.h"
#include <stdio.h>
#include <ctime>
#include <windows.h>
#include <cstdlib>

using std::cout;
using std::endl;

const int SCREEN_WIDTH = 350;
const int SCREEN_HEIGHT = 700;
const int GBOARD_WIDTH = 10;
const int GBOARD_HEIGHT = 22; //rows 0 and 1 is for new pieces to spawn into

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
TTF_Font* gfont = NULL;

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

class cgame{
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
    SDL_Rect board_viewport;
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
    void print_npboard();
    void print_gboard();
    void print_cur_piece_info();
    bool check_game_over();
    void init_board();
    void draw_pieces();
    void draw_ghost();
    void blank_board();
    void game_over_screen();
    void restart();
    DWORD current_time = 0, last_time = 0, down_timer = 0, lr_timer = 0, lock_timer = 0;
    DWORD down_time = 35, lr_delay_time = 150, difficulty = 1000, lock_delay = 500, lr_speed = 35;
    bool space_pressed = false, lr_pressed = false, rotate_pressed = false, lock = false, lr_delay = true;
};

cgame::cgame()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout << "Warning: Linear Texture filtering not enabled";
        }

        gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            cout << "Window could not be created! SDL Error: " << SDL_GetError();
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError();
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                if(TTF_Init() == -1)
                {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
                }
            }
        }
    }

    clear_board(gboard);

    clear_board(npboard);

    new_shape();
}

cgame::~cgame()
{
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    TTF_CloseFont(gfont);

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    TTF_Quit();
    SDL_Quit();
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

void cgame::init_board()
{
    SDL_Rect tempRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &tempRect);

    gfont = TTF_OpenFont("basictitlefont.ttf", 20);
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    SDL_Surface* txtSurface = TTF_RenderText_Solid(gfont, "Hit Enter to restart when game over", textColor);
    gTexture = SDL_CreateTextureFromSurface(gRenderer, txtSurface);
    tempRect = {10, 10, SCREEN_WIDTH-20, 30};
    SDL_RenderCopy(gRenderer, gTexture, NULL, &tempRect);


    tempRect = {25, 50, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 100};
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(gRenderer, &tempRect);

    SDL_RenderPresent(gRenderer);

    tempRect.h = SCREEN_HEIGHT-101;
    tempRect.w = SCREEN_WIDTH-51;

    SDL_RenderSetViewport(gRenderer, &tempRect);

    tempRect.h = ((SCREEN_HEIGHT-100)/20);
    tempRect.w = ((SCREEN_WIDTH-50)/10);

    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            tempRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            tempRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
            SDL_RenderDrawRect(gRenderer, &tempRect);
        }
    }
}

void cgame::draw_pieces()
{
    blank_board();

    SDL_Rect blockRect;
    blockRect.h = ((SCREEN_HEIGHT-100)/20);
    blockRect.w = ((SCREEN_WIDTH-50)/10);
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
                blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
                blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect(gRenderer, &blockRect);
                SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
                SDL_RenderDrawRect(gRenderer, &blockRect);
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
                blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
                blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x80);
                SDL_RenderFillRect(gRenderer, &blockRect);
                SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
                SDL_RenderDrawRect(gRenderer, &blockRect);
            }
        }
    }

    cur_piece = initial_state;
    clear_board(npboard);
    print_piece2npboard(cur_piece);
}

void cgame::blank_board()
{
    SDL_Rect boardRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &boardRect);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(gRenderer, &boardRect);

    boardRect.h = ((SCREEN_HEIGHT-100)/20);
    boardRect.w = ((SCREEN_WIDTH-50)/10);

    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            boardRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            boardRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
            SDL_RenderDrawRect(gRenderer, &boardRect);
        }
    }
}

void cgame::game_over_screen()
{
    SDL_Rect screen_size = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderSetViewport(gRenderer, &screen_size);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0x80);
    SDL_RenderFillRect(gRenderer, &screen_size);

    SDL_RenderPresent(gRenderer);
}

void cgame::restart()
{
    game_over = false;

    clear_board(gboard);
    clear_board(npboard);

    new_shape();

    SDL_RenderClear(gRenderer);
    init_board();
}

int main(int argv, char* argc[])
{
    srand(time(NULL));
    SDL_Event e;
    SDL_Keycode keypressed = 0;
    SDL_Keycode last_keypressed = 0;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    cgame Game;

    bool quit = false;

    Game.init_board();

    while(!quit)
    {
        if(Game.check_game_over())
        {
            Game.game_over_screen();

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
                    Game.restart();
                    break;
                }
            }
        }

        Game.current_time = timeGetTime();

        if((Game.current_time - Game.last_time) >= Game.difficulty)
        {
            Game.shape_down();
            //Game.print_cur_piece_info();
            Game.last_time = Game.current_time;
            Game.draw_pieces();
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
            if((timeGetTime() - Game.down_timer) >= Game.down_time)
            {
                Game.shape_down();
                //Game.print_cur_piece_info();
                Game.down_timer = timeGetTime();
                Game.last_time = timeGetTime();
                Game.draw_pieces();
            }
        }

        if(Game.lr_pressed)
        {
            if(currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
            {
                if(Game.lr_delay)
                {
                    if((timeGetTime() - Game.lr_timer) >= Game.lr_delay_time)
                    {
                        Game.shape_right();
                        //Game.print_cur_piece_info();
                        Game.lr_timer = timeGetTime();
                        Game.draw_pieces();
                        Game.lr_delay = false;
                    }
                }
                if(!Game.lr_delay)
                {
                    if((timeGetTime() - Game.lr_timer) >= (Game.lr_speed))
                    {
                        Game.shape_right();
                        //Game.print_cur_piece_info();
                        Game.lr_timer = timeGetTime();
                        Game.draw_pieces();
                        Game.lr_delay = false;
                    }
                }
            }
            if(!currentKeyStates[SDL_SCANCODE_RIGHT] && currentKeyStates[SDL_SCANCODE_LEFT])
            {
                if(Game.lr_delay)
                {
                    if((timeGetTime() - Game.lr_timer) >= Game.lr_delay_time)
                    {
                        Game.shape_left();
                        //Game.print_cur_piece_info();
                        Game.lr_timer = timeGetTime();
                        Game.draw_pieces();
                        Game.lr_delay = false;
                    }
                }
                if(!Game.lr_delay)
                {
                    if((timeGetTime() - Game.lr_timer) >= (Game.lr_speed))
                    {
                        Game.shape_left();
                        //Game.print_cur_piece_info();
                        Game.lr_timer = timeGetTime();
                        Game.draw_pieces();
                        Game.lr_delay = false;
                    }
                }
            }
        }
        if(!Game.lr_pressed)
        {
            if(currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
            {
                Game.shape_right();
                Game.draw_pieces();
                Game.lr_timer = timeGetTime();
                Game.lr_pressed = true;
            }

            if(currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                Game.shape_left();
                //Game.print_cur_piece_info();
                Game.lr_timer = timeGetTime();
                Game.draw_pieces();
                Game.lr_pressed = true;
            }
        }
        if(!currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_LEFT])
        {
            Game.lr_pressed = false;
            Game.lr_delay = true;
        }
        if(Game.space_pressed)
        {
            if(!currentKeyStates[SDL_SCANCODE_SPACE])
            {
                Game.space_pressed = false;
            }
        }
        if(!Game.space_pressed)
        {
            if(currentKeyStates[SDL_SCANCODE_SPACE])
            {
                Game.space_pressed = true;
                Game.hard_drop();
                //Game.print_cur_piece_info();
                Game.last_time = timeGetTime();
                Game.draw_pieces();
            }
        }
        if(Game.rotate_pressed)
        {
            if(!currentKeyStates[SDL_SCANCODE_Z] && !currentKeyStates[SDL_SCANCODE_X])
            {
                Game.rotate_pressed = false;
            }
        }
        if(!Game.rotate_pressed)
        {
            if(currentKeyStates[SDL_SCANCODE_Z] && !currentKeyStates[SDL_SCANCODE_X])
            {
                Game.rotate_pressed = true;
                Game.CCW_rotate();
                //Game.print_cur_piece_info();
                Game.draw_pieces();
            }
            if(currentKeyStates[SDL_SCANCODE_X] && !currentKeyStates[SDL_SCANCODE_Z])
            {
                Game.rotate_pressed = true;
                Game.CW_rotate();
                //Game.print_cur_piece_info();
                Game.draw_pieces();
            }
        }

        SDL_RenderPresent(gRenderer);
    }

    return 0;
}
