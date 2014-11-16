#include "gamegraphics.h"

using std::cout;
using std::endl;

Graphics::Graphics() : SDLGraphics(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone")
{
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
}

Graphics::~Graphics()
{
    TTF_CloseFont(gfont);
}

void Graphics::init_board_graphics()
{
    SDL_RenderClear(gRenderer);

    SDL_Rect tempRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    //Fill screen with black
    blankScreen(BLACKRGB, BLACKRGB, BLACKRGB);

    //Draw Text
    gfont = TTF_OpenFont("basictitlefont.ttf", 20);
    SDL_Color textColor = {WHITERGB, WHITERGB, WHITERGB};
    SDL_Surface* txtSurface = TTF_RenderText_Solid(gfont, "Hit Enter to restart when game over", textColor);
    gTexture = SDL_CreateTextureFromSurface(gRenderer, txtSurface);
    tempRect = {10, 10, SCREEN_WIDTH-20, 30};
    SDL_RenderCopy(gRenderer, gTexture, NULL, &tempRect);
    SDL_FreeSurface(txtSurface);

    //Draw board box
    RectDraw(boardSize, WHITERGB);

    render();

    tempRect = {boardSize.x+1, boardSize.y+1, boardSize.w-2, boardSize.h-2};

    SDL_RenderSetViewport(gRenderer, &tempRect);

    //Draw Grid
    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            RectDraw(blockRect, GRIDRGB, GRIDALPHA);
        }
    }
}

void Graphics::draw_solid_block(int i, int j)
{
    blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
    blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
    RectFill(blockRect, WHITERGB);
    RectDraw(blockRect, GRIDRGB, GRIDALPHA);
}

void Graphics::draw_ghost_block(int i, int j)
{
    blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
    blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
    RectFill(blockRect, GHOSTRGB, GHOSTALPHA);
    RectDraw(blockRect, GRIDRGB, GRIDALPHA);
}

void Graphics::blank_board()
{
    //Blank Screen
    blankScreen(BLACKRGB);

    //Redraw box
    RectDraw(boardRect, WHITERGB);

    //Redraw Grid
    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            RectDraw(blockRect, GRIDRGB, GRIDALPHA);
        }
    }
}

void Graphics::game_over_screen()
{
    RectFill(boardRect, 0xFF, 0x00, 0x00, 0x80);

    render();
}
