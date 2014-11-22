
set(SDL_STATIC    OFF CACHE BOOL "SDL static build" FORCE)
set(SDL_SHARED    ON  CACHE BOOL "SDL shared build" FORCE)

set(DIRECTX       OFF CACHE BOOL "SDL use directx" FORCE)
set(RENDER_D3D    OFF CACHE BOOL "SDL use d3d" FORCE)

add_subdirectory(SDL2-2.0.3)
