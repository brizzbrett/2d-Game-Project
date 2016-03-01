#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "simple_logger.h"

SDL_Surface *buffer;		/**<pointer to the background image buffer*/
SDL_Surface *videobuffer;	/**<pointer to the draw buffer*/
SDL_Rect Camera;			/**<x & y are the coordinates for the background map, w and h are of the screen*/
Uint32 NOW;					/**<the current time since program started*/

static SDL_Window   *   __graphics_main_window = NULL;
static SDL_Renderer *   __graphics_renderer = NULL;
static SDL_Texture  *   __graphics_texture = NULL;
static SDL_Surface  *   __graphics_surface = NULL;
static SDL_Surface  *   __graphics_temp_buffer = NULL;

/**timing*/
static Uint32 graphics_frame_delay = 30;
static Uint32 graphics_now = 0;
static Uint32 graphics_then = 0;
static Uint8 graphics_print_fps = 1;
static float graphics_fps = 0; 

/**some data on the video settings that can be useful for a lot of functions*/
static int bitdepth;
static Uint32 rmask;
static Uint32 gmask;
static Uint32 bmask;
static Uint32 amask;

void Graphics_Close();

void Graphics_Init(char *windowName,int viewW,int viewH,int renderW,int renderH,float bgcolor[4],int fullscreen)
{
    Uint32 flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        slog("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (renderW == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }
    __graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderW, renderH,
                             flags);

    if (!__graphics_main_window)
    {
        slog("failed to create main window: %s",SDL_GetError());
        Graphics_Close();
        return;
    }
    
    __graphics_renderer = SDL_CreateRenderer(__graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!__graphics_renderer)
    {
        slog("failed to create renderer: %s",SDL_GetError());
        Graphics_Close();
        return;
    }
    
    SDL_SetRenderDrawColor(__graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__graphics_renderer);
    SDL_RenderPresent(__graphics_renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(__graphics_renderer, renderW, renderH);

    __graphics_texture = SDL_CreateTexture(
        __graphics_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        renderW, renderH);
    if (!__graphics_texture)
    {
        slog("failed to create screen texture: %s",SDL_GetError());
        Graphics_Close();
        return;
    };
    
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888,
                                    &bitdepth,
                                    &rmask,
                                    &gmask,
                                    &bmask,
                                    &amask);

    
    __graphics_surface = SDL_CreateRGBSurface(0, renderW, renderH, bitdepth,
                                        rmask,
                                    gmask,
                                    bmask,
                                    amask);
    buffer = SDL_CreateRGBSurface(0, renderW, renderH, bitdepth,
                                                 rmask,
                                                 gmask,
                                                 bmask,
                                                 amask);    
    if (!__graphics_surface)
    {
        slog("failed to create screen surface: %s",SDL_GetError());
        Graphics_Close();
        return;
    }
        
    atexit(Graphics_Close);
    slog("graphics initialized\n");
}

void Graphics_RenderSurfaceToScreen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y)
{
    SDL_Rect dstRect;
    SDL_Point point = {1,1};
    int w,h;
    if (!__graphics_texture)
    {
        slog("Graphics_RenderSurfaceToScreen: no texture available");
        return;
    }
    if (!surface)
    {
        slog("Graphics_RenderSurfaceToScreen: no surface provided");
        return;
    }
    SDL_QueryTexture(__graphics_texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed*/
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(__graphics_texture);
        __graphics_texture = SDL_CreateTexture(__graphics_renderer,
                                                   __graphics_surface->format->format,
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   surface->w,
                                                   surface->h);
        if (!__graphics_texture)
        {
            slog("Graphics_RenderSurfaceToScreen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(__graphics_texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(__graphics_texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
    SDL_RenderCopy(__graphics_renderer,
                     __graphics_texture,
                     &srcRect,
                     &dstRect);
}


void ResetBuffer()
{
    
}

void Graphics_FrameDelay()
{
    Uint32 diff;
    graphics_then = graphics_now;
    graphics_now = SDL_GetTicks();
    diff = (graphics_now - graphics_then);
    if (diff < graphics_frame_delay)
    {
        SDL_Delay(graphics_frame_delay - diff);
    }
    graphics_fps = 1000.0/MAX(SDL_GetTicks() - graphics_then,0.001);
    if (graphics_print_fps)
    {
        //slog("FPS: %f",graphics_fps);
    }
}

void NextFrame()
{
  SDL_RenderPresent(__graphics_renderer);
  Graphics_FrameDelay();
}

void Graphics_Close()
{
    if (__graphics_texture)
    {
        SDL_DestroyTexture(__graphics_texture);
    }
    if (__graphics_renderer)
    {
        SDL_DestroyRenderer(__graphics_renderer);
    }
    if (__graphics_main_window)
    {
        SDL_DestroyWindow(__graphics_main_window);
    }
    if (__graphics_surface)
    {
        SDL_FreeSurface(__graphics_surface);
    }
    if (__graphics_temp_buffer)
    {
        SDL_FreeSurface(__graphics_temp_buffer);
    }
    __graphics_surface = NULL;
    __graphics_main_window = NULL;
    __graphics_renderer = NULL;
    __graphics_texture = NULL;
    __graphics_temp_buffer = NULL;
}

SDL_Renderer *Graphics_GetActiveRenderer()
{
    return __graphics_renderer;
}

Uint32 Graphics_GetSystemTime()
{
    return graphics_now;
}