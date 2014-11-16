#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#define WHITERGB 0xFF
#define BLACKRGB 0x00
#define FULLALPHA 0xFF

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

class LTexture
{
public:
    LTexture();
    ~LTexture();
    void freeTex();
protected:
    SDL_Texture* gTexture;
    int texWidth;
    int texHeight;
};

class SDLGraphics : public LTexture
{
public:
    SDLGraphics(int windowWidth, int windowHeight, std::string windowTitle);
    ~SDLGraphics();
    void blankScreen(int bgRGB);
    void blankScreen(int bgR, int bgG, int bgB);
    void RectFill(SDL_Rect dstRect, int RGB, int alpha = FULLALPHA);
    void RectFill(SDL_Rect dstRect, int R, int G, int B, int alpha = FULLALPHA);
    void RectDraw(SDL_Rect dstRect, int RGB, int alpha = FULLALPHA);
    void RectDraw(SDL_Rect dstRect, int R, int G, int B, int alpha = FULLALPHA);
    void loadTex(std::string path);
    void copyTex(int x, int y);
    void render();
protected:
    SDL_Rect screenRect;
    SDL_Renderer* gRenderer;
    SDL_Window* gWindow;
};

#endif // GRAPHICS_HEADER
