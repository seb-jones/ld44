typedef struct Texture
{
    SDL_Texture *handle;
    int          width; 
    int          height;
}
Texture;

Texture *load_bitmap(const char *filename)
{
    SDL_Surface *surface = SDL_LoadBMP(filename);
    if (!surface)
        return NULL;

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));

    Texture *texture = malloc(sizeof(*texture));
    texture->handle = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    texture->width = surface->w;
    texture->height = surface->h;

    if (!texture->handle) {
        free(texture);
        return NULL;
    }

    SDL_FreeSurface(surface);

    return texture;
}

void draw_texture(Texture *texture, int x, int y)
{
    SDL_Rect destination = { x, y, texture->width, texture->height };

    SDL_RenderCopy(sdl.renderer, texture->handle, NULL, &destination);
}

Texture *destroy_texture(Texture *texture)
{
    SDL_DestroyTexture(texture->handle);
    free(texture);
    return NULL;
}
