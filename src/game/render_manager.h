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

class RenderManager : public base::Singleton<RenderManager>
{
public:
    RenderManager(Game* game);
    virtual ~RenderManager();

    void Render();

    RenderLayer* addLayer(const std::string& name, int priority);
    void showLayer(const std::string& name);
    void hideLayer(const std::string& name);
    RenderLayer* getLayer(const std::string& name);
private:
    void refreshRenderList();

    typedef std::map<std::string, RenderLayer*> LayerMap;
    typedef std::vector<RenderLayer*> LayerList;

    LayerMap layers_;
    LayerList renderList_;
    SDL_Renderer* renderer_;
};
}
