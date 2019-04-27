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

u32 argb_to_hex(u8 a, u8 r, u8 g, u8 b)
{
    return b | (g << 8) | (r << 16) | (a << 24);
}

void set_hex_color(u32 color)
{
    u8 a, r, g, b;
    hex_to_argb(color, &a, &r, &g, &b);
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
}

double inverse_lerp_int(int a, int b, int t)
{
    return (double)(t - a) / (double)(b - a);
}

u8 lerp_u8(u8 a, u8 b, double t)
{
    return a + (u8)((double)(b - a) * t);
}

u32 lerp_color(u32 color_1, u32 color_2, double t)
{
    u8 a1, r1, g1, b1;
    u8 a2, r2, g2, b2;

    hex_to_argb(color_1, &a1, &r1, &g1, &b1);
    hex_to_argb(color_2, &a2, &r2, &g2, &b2);

    u8 a, r, g, b;

    a = lerp_u8(a1, a2, t);
    r = lerp_u8(r1, r2, t);
    g = lerp_u8(g1, g2, t);
    b = lerp_u8(b1, b2, t);

    return argb_to_hex(a, r, g, b);
}

void draw_rectangle(int x, int y, int width, int height)
{
    SDL_Rect rectangle = { x, y, width, height };
    SDL_RenderFillRect(sdl.renderer, &rectangle);
}
