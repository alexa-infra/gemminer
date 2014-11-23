#pragma once

#include "base/singleton.h"
#include <string>
#include <map>

struct SDL_Texture;
struct SDL_Renderer;

namespace game {

    class Game;

    struct ImageTexture
    {
        SDL_Texture* texture;
        int w;
        int h;
    };

    class ResourceManager : public ::base::Singleton<ResourceManager>
    {
    public:
        ResourceManager(Game* app);
        virtual ~ResourceManager();
        ImageTexture Texture(const std::string& path);
    private:
        ImageTexture LoadTexture(const std::string& path);
    private:
        typedef std::map<std::string, ImageTexture> TextureMap;
        TextureMap textures_;
        SDL_Renderer* renderer_;
    };

} // namespace game
