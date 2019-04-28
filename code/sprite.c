typedef struct Sprite
{
    Texture    *texture;
    int         x;
    int         y;
    int         width;
    int         height;
    int         vel_x;
    u32         color;
    u8          alpha;
    bool        visible;
}
Sprite;

Sprite *load_sprite(const char *filename)
{
    Sprite *sprite = malloc(sizeof(*sprite));

    sprite->texture = load_bitmap(filename);

    if (!sprite->texture) {
        free(sprite);
        return NULL;
    }

    sprite->x = 0;
    sprite->y = 0;
    sprite->color = 0xffffffff;
    sprite->alpha = 0xff;
    sprite->width = sprite->texture->width;
    sprite->height = sprite->texture->height;
    sprite->visible = true;
    sprite->vel_x = 0;

	return sprite;
}

void update_sprite(Sprite *sprite)
{
    if (sprite->vel_x != 0)
        sprite->x += sprite->vel_x;
}

void draw_sprite(Sprite *sprite)
{
    if (!sprite->visible)
        return;

    u8 r, g, b;
    hex_to_argb(sprite->color, NULL, &r, &g, &b);

    SDL_SetTextureColorMod(sprite->texture->handle, r, g, b);

    SDL_SetTextureAlphaMod(sprite->texture->handle, sprite->alpha);

    draw_texture(sprite->texture, sprite->x, sprite->y);
}
