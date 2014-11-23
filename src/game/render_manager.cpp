#include "game/render_manager.h"
#include "game/game.h"
#include <SDL.h>
#include <algorithm>
#include <cmath>

using namespace game;
using namespace base;

template<> RenderManager * Singleton<RenderManager>::instance_ = nullptr;

RenderManager::RenderManager(Game* game)
{
    renderer_ = game->renderer();
}

RenderManager::~RenderManager()
{
    LayerMap::iterator it;
    for (it = layers_.begin(); it != layers_.end(); ++it)
    {
        delete it->second;
    }
}

void RenderManager::Render()
{
    LayerList::iterator it;
    for (it = renderList_.begin(); it != renderList_.end(); ++it)
    {
        RenderLayer* layer = *it;
        std::vector<Sprite*>::iterator s;
        for (s = layer->sprites.begin(); s != layer->sprites.end(); ++s)
        {
            Sprite* sprite = *s;
            SDL_SetTextureColorMod(sprite->tex,
                (u8)floor(sprite->color.r * 255),
                (u8)floor(sprite->color.g * 255),
                (u8)floor(sprite->color.b * 255));
            SDL_SetTextureAlphaMod(sprite->tex, (u8)floor(sprite->color.a * 255));
            SDL_Rect rect;
            rect.x = (i32)sprite->position.x;
            rect.y = (i32)sprite->position.y;
            rect.w = sprite->w;
            rect.h = sprite->h;
            SDL_RenderCopy(renderer_, sprite->tex, NULL, &rect);
        }
    }
}

RenderLayer* RenderManager::addLayer(const std::string& name, int priority)
{
    RenderLayer* layer = new RenderLayer;
    layer->name = name;
    layer->priority = priority;
    layer->visible = true;
    layers_[name] = layer;
    refreshRenderList();
    return layer;
}

void RenderManager::showLayer(const std::string& name)
{
    RenderLayer* layer = layers_[name];
    layer->visible = true;
    refreshRenderList();
}

void RenderManager::hideLayer(const std::string& name)
{
    RenderLayer* layer = layers_[name];
    layer->visible = false;
    refreshRenderList();
}

RenderLayer* RenderManager::getLayer(const std::string& name)
{
    return layers_[name];
}

bool CmpLayers(const RenderLayer* a, const RenderLayer* b) {
    return a->priority < b->priority;
}

void RenderManager::refreshRenderList()
{
    renderList_.clear();
    LayerMap::iterator it;
    for (it = layers_.begin(); it != layers_.end(); ++it)
    {
        RenderLayer* layer = it->second;
        if (!layer->visible)
            continue;
        renderList_.push_back(layer);
    }
    std::sort(renderList_.begin(), renderList_.end(), CmpLayers);
}
