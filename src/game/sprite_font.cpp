#include "game/sprite_font.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "SDL.h"

using namespace game;

SpriteFont::SpriteFont(SDL_Renderer* renderer, const std::string& filename)
{
    renderer_ = renderer;
    fontHeight_ = 36;

    SDL_RWops* f = SDL_RWFromFile(filename.c_str(), "rb");
    i32 size = (i32)SDL_RWsize(f);
    u8* data = new u8[size];
    SDL_RWread(f, data, size, 1);
    SDL_RWclose(f);

    const int w = textureSize_;
    const int h = textureSize_;

    u8* bitmap = new u8[w * h];

    cdata_ = new stbtt_bakedchar[96];
    stbtt_BakeFontBitmap((const u8*)data,
                         0,
                         fontHeight_,
                         bitmap,
                         w,
                         h,
                         32,
                         96,
                         (stbtt_bakedchar*)cdata_);
    delete[] data;


    // SDL does not understand GL_ALPHA :(
    u8* bitmap2 = new u8[w * h * 4];
    i32 ss = w * h;
    for (int i = 0; i < ss; i++) {
        bitmap2[i * 4 + 0] = 0xff;
        bitmap2[i * 4 + 1] = 0xff;
        bitmap2[i * 4 + 2] = 0xff;
        bitmap2[i * 4 + 3] = bitmap[i];
    }
    delete bitmap;

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(bitmap2, w, h,
                           4 * 8, w * 4, 0xff, 0xff00, 0xff0000, 0xff000000);
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureColorMod(texture_, 0xff, 0xff, 0xff);

    delete bitmap2;

    SDL_FreeSurface(surface);
}

SpriteFont::~SpriteFont()
{
    SDL_DestroyTexture(texture_);
    delete[] cdata_;
}

void SpriteFont::RenderText(const std::string& text, float x, float y)
{
    float xStart = x;
    u32 textLen = (u32)text.size();
    for (u32 i = 0; i < textLen; i++) {
        u8 ch = text[i];

        if (ch == '\n') {
            x = xStart;
            y += fontHeight_;
            continue;
        }

        if (ch < 32 || ch > 128)
            ch = '?';

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata_,
                           textureSize_,
                           textureSize_,
                           ch - 32,  // position of character in font
                           &x,     // current position
                           &y,
                           &q,     // resulted quad
                           1);     // '1' for tex coords for opengl ('0' for d3d)

        f32 w = q.x1 - q.x0;
        f32 h = q.y1 - q.y0;

        SDL_Rect rect;
        rect.x = (int)q.x0;
        rect.y = (int)q.y0;
        rect.w = (int)w;
        rect.h = (int)h;

        SDL_Rect tex;
        tex.x = (int)(q.s0 * 512);
        tex.y = (int)(q.t0 * 512);
        tex.w = (int)((q.s1 - q.s0) * 512);
        tex.h = (int)((q.t1 - q.t0) * 512);

        SDL_RenderCopy(renderer_, texture_, &tex, &rect);
    }
}
