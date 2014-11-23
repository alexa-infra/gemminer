#pragma once

#include "base/types.h"
#include <string>
#include "stb_truetype.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace game
{

class SpriteFont
{
public:
    SpriteFont(SDL_Renderer* renderer, const std::string& filename);
    ~SpriteFont();

    void RenderText(const std::string& text, float x, float y);

private:
    u32 max_chars_;             //!< maximum text length
    u32 text_length_;           //!< current text length
    f32 font_height_;           //!< current font height

    SDL_Renderer* renderer_;
    SDL_Texture* texture_;      //!< font texture
    stbtt_bakedchar* cdata_;    //!< font data: ASCII 32..126 is 95 glyphs
};

} // namespace game
