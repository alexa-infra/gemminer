#pragma once

#include "base/singleton.h"
#include <string>
#include <map>

struct SDL_Texture;
struct SDL_Renderer;

namespace game {

class Game;
class SpriteFont;

struct ImageTexture
{
    SDL_Texture* texture;
    int w;
    int h;
};

//! Resource cache and resource loader
class ResourceManager : public ::base::Singleton<ResourceManager>
{
public:
    ResourceManager(Game* app);
    virtual ~ResourceManager();
    ImageTexture Texture(const std::string& path);
    SpriteFont* Font(const std::string& path);
private:
    SpriteFont* LoadFont(const std::string& path);
    ImageTexture LoadTexture(const std::string& path);
private:
    typedef std::map<std::string, SpriteFont*> FontMap;
    typedef std::map<std::string, ImageTexture> TextureMap;
    TextureMap textures_;
    FontMap fonts_;
    SDL_Renderer* renderer_;
    std::string base_;
};

} // namespace game
