#pragma once

#include "base/types.h"
#include <string>
#include "stb_truetype.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace game
{

//! Simple font renderer (based on ttf font file)
//!   it doesn't make batch from draw calls (each character is rendered separately from atlas)
class SpriteFont
{
public:
    SpriteFont(SDL_Renderer* renderer, const std::string& filename);
    ~SpriteFont();

    //! Renders text at screen
    void RenderText(const std::string& text, float x, float y);

private:
    f32 fontHeight_;           //!< current font height
    i32 textureSize_;           //!< texture size

    SDL_Renderer* renderer_;
    SDL_Texture* texture_;      //!< font texture
    stbtt_bakedchar* cdata_;    //!< font data: ASCII 32..126 is 95 glyphs
};

} // namespace game
