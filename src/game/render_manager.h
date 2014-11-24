#pragma once

#include <vector>
#include <map>
#include <string>
#include "base/singleton.h"
#include "game/sprite.h"

struct SDL_Renderer;

namespace game
{
class Game;

struct RenderLayer
{
    std::string name;
    int priority;
    bool visible;
    std::vector<Sprite*> sprites;
};

//! Sprite renderer
//!   Renders layers according to their order and visibility
//!   Sprites inside layer are not sorted
class RenderManager : public base::Singleton<RenderManager>
{
public:
    RenderManager(Game* game);
    virtual ~RenderManager();

    void Render();

    RenderLayer* AddLayer(const std::string& name, int priority);
    void ShowLayer(const std::string& name);
    void HideLayer(const std::string& name);
    RenderLayer* GetLayer(const std::string& name);
private:
    void RefreshRenderList();

    typedef std::map<std::string, RenderLayer*> LayerMap;
    typedef std::vector<RenderLayer*> LayerList;

    LayerMap layers_;
    LayerList renderList_;
    SDL_Renderer* renderer_;
};
}
