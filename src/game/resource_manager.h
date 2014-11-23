#pragma once

#include "base/singleton.h"
#include <string>
#include <map>

struct SDL_Texture;
struct SDL_Renderer;

namespace game {

    class Game;

    class ResourceManager : public ::base::Singleton<ResourceManager>
    {
    public:
        ResourceManager(Game* app);
        virtual ~ResourceManager();
        SDL_Texture* Texture(const std::string& path);
    private:
        SDL_Texture* LoadTexture(const std::string& path);
    private:
        typedef std::map<std::string, SDL_Texture*> TextureMap;
        TextureMap textures_;
        SDL_Renderer* renderer_;
    };

} // namespace game
