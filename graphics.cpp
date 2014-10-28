#include "graphics.h"

using std::cout;
using std::endl;

Graphics::Graphics()
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
}

Graphics::~Graphics()
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

void Graphics::init_board_graphics()
{
    SDL_RenderClear(gRenderer);

    //Fill screen with black
    SDL_Rect tempRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &tempRect);

    //Draw Text
    gfont = TTF_OpenFont("basictitlefont.ttf", 20);
    SDL_Color textColor = {0xFF, 0xFF, 0xFF};
    SDL_Surface* txtSurface = TTF_RenderText_Solid(gfont, "Hit Enter to restart when game over", textColor);
    gTexture = SDL_CreateTextureFromSurface(gRenderer, txtSurface);
    tempRect = {10, 10, SCREEN_WIDTH-20, 30};
    SDL_RenderCopy(gRenderer, gTexture, NULL, &tempRect);

    //Draw board box
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(gRenderer, &boardSize);

    SDL_RenderPresent(gRenderer);

    tempRect = {boardSize.x+1, boardSize.y+1, boardSize.w-2, boardSize.h-2};

    SDL_RenderSetViewport(gRenderer, &tempRect);

    //Draw Grid
    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
            SDL_RenderDrawRect(gRenderer, &blockRect);
        }
    }
}

void Graphics::draw_solid_block(int i, int j)
{
    blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
    blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &blockRect);
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
    SDL_RenderDrawRect(gRenderer, &blockRect);
}

void Graphics::draw_ghost_block(int i, int j)
{
    blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
    blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x80);
    SDL_RenderFillRect(gRenderer, &blockRect);
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
    SDL_RenderDrawRect(gRenderer, &blockRect);
}

void Graphics::blank_board()
{
    //Blank Screen
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &boardRect);

    //Redraw box
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(gRenderer, &boardRect);

    //Redraw Grid
    for(int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for(int j = 2; j < GBOARD_WIDTH+2; j++)
        {
            blockRect.x = (j-2)*((SCREEN_WIDTH-50)/10);
            blockRect.y = (i-2)*((SCREEN_HEIGHT-100)/20);
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0x80);
            SDL_RenderDrawRect(gRenderer, &blockRect);
        }
    }
}

void Graphics::render_screen()
{
    SDL_RenderPresent(gRenderer);
}

void Graphics::game_over_screen()
{
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0x80);
    SDL_RenderFillRect(gRenderer, &boardRect);

    SDL_RenderPresent(gRenderer);
}
