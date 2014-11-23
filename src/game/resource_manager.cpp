#include "game/resource_manager.h"
#include "game/game.h"

#define STBI_FAILURE_USERMSG
#include "stb_image.c"

#include "SDL.h"

using namespace game;
using namespace base;

struct TextureInfo
{
    i32 Width;
    i32 Height;
    i32 ComponentCount;
};

struct StbiImage
{
    StbiImage(const std::string& path, TextureInfo& info) : buffer(NULL)
    {
        buffer = stbi_load(path.c_str(), &info.Width, &info.Height, &info.ComponentCount, 0);
    }
    ~StbiImage()
    {
        if (buffer != NULL)
            stbi_image_free(buffer);
    }
    u8* buffer;
    bool isOk() const { return buffer != NULL; }
};

template<> ResourceManager* Singleton<ResourceManager>::instance_ = nullptr;

ResourceManager::ResourceManager(Game* app)
{
    renderer_ = app->renderer();
}

ResourceManager::~ResourceManager()
{
    TextureMap::const_iterator it;
    for (it = textures_.begin(); it != textures_.end(); ++it)
    {
        const ImageTexture& img = it->second;
        SDL_DestroyTexture(img.texture);
    }
}

ImageTexture ResourceManager::LoadTexture(const std::string& path)
{
    ImageTexture ret;
    ret.texture = nullptr;
    ret.w = 0;
    ret.h = 0;

    TextureInfo info;
    StbiImage image(path, info);
    if (!image.isOk()) {
        return ret;
    }
    int rmask = 0, gmask = 0, bmask = 0, amask = 0;
    if (info.ComponentCount == 3)
    {
        rmask = 0x0000ff;
        gmask = 0x00ff00;
        bmask = 0xff0000;
        amask = 0;
    }
    else if (info.ComponentCount == 4)
    {
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(image.buffer, info.Width, info.Height,
        info.ComponentCount * 8, info.Width * info.ComponentCount, rmask, gmask, bmask, amask);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    ret.texture = tex;
    ret.w = info.Width;
    ret.h = info.Height;
    return ret;
}

ImageTexture ResourceManager::Texture(const std::string& path)
{
    TextureMap::iterator it = textures_.find(path);
    if (it != textures_.end())
        return it->second;
    ImageTexture tex = LoadTexture(path);
    textures_[path] = tex;
    return tex;
}
