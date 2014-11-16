#include "graphics.h"

using std::cout;
using std::endl;

LTexture::LTexture()
{
    gTexture = NULL;
    texHeight = 0;
    texWidth = 0;
}

LTexture::~LTexture()
{
    freeTex();
}

void LTexture::freeTex()
{
    if(gTexture != NULL)
    {
        SDL_DestroyTexture(gTexture);
        gTexture = NULL;
        texHeight = 0;
        texWidth = 0;
    }
}

SDLGraphics::SDLGraphics(int windowWidth, int windowHeight, std::string windowTitle)
{
    gRenderer = NULL;
    gWindow = NULL;

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

        gWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            cout << "Window could not be created! SDL Error: " << SDL_GetError();
        }
        else
        {
            screenRect = {0, 0, windowWidth, windowHeight};

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

                int IMGFlags = IMG_INIT_PNG|IMG_INIT_JPG|IMG_INIT_TIF;

                if(!(IMG_Init(IMGFlags) & IMGFlags))
                {
                    cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
                }
            }
        }
    }
}

SDLGraphics::~SDLGraphics()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDLGraphics::blankScreen(int bgRGB)
{
    blankScreen(bgRGB, bgRGB, bgRGB);
}

void SDLGraphics::blankScreen(int bgR, int bgG, int bgB)
{
    SDL_SetRenderDrawColor(gRenderer, bgR, bgG, bgB, FULLALPHA);
    SDL_RenderFillRect(gRenderer, &screenRect);
}

void SDLGraphics::loadTex(std::string path)
{
    freeTex();

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface = NULL)
    {
        cout << "Failed to load " << path << ". SDL_image Error " << IMG_GetError() << endl;
    }
    else
    {
        texHeight = loadedSurface->h;
        texWidth = loadedSurface->w;

        gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        SDL_FreeSurface(loadedSurface);
    }
}

void SDLGraphics::RectFill(SDL_Rect dstRect, int RGB, int alpha)
{
    RectFill(dstRect, RGB, RGB, RGB, alpha);
}

void SDLGraphics::RectFill(SDL_Rect dstRect, int R, int G, int B, int alpha)
{
    SDL_SetRenderDrawColor(gRenderer, R, G, B, alpha);
    SDL_RenderFillRect(gRenderer, &dstRect);
}

void SDLGraphics::RectDraw(SDL_Rect dstRect, int RGB, int alpha)
{
    RectDraw(dstRect, RGB, RGB, RGB, alpha);
}

void SDLGraphics::RectDraw(SDL_Rect dstRect, int R, int G, int B, int alpha)
{
    SDL_SetRenderDrawColor(gRenderer, R, G, B, alpha);
    SDL_RenderDrawRect(gRenderer, &dstRect);
}

void SDLGraphics::copyTex(int x, int y)
{
    SDL_Rect rendRect = {x, y, texWidth, texHeight};

    SDL_RenderCopy(gRenderer, gTexture, NULL, &rendRect);
}

void SDLGraphics::render()
{
    SDL_RenderPresent(gRenderer);
}
