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

void hex_to_argb(u32 hex, u8 *a, u8 *r, u8 *g, u8 *b)
{
    if (b) *b = hex;
    if (g) *g = hex >> 8;
    if (r) *r = hex >> 16;
    if (a) *a = hex >> 24;
}

void set_hex_color(u32 color)
{
    u8 a, r, g, b;
    hex_to_argb(color, &a, &r, &g, &b);
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
}

void draw_rectangle(int x, int y, int width, int height)
{
    SDL_Rect rectangle = { x, y, width, height };
    SDL_RenderFillRect(sdl.renderer, &rectangle);
}
