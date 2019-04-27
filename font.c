typedef struct Font
{
    Texture *texture;
    int glyph_width;
    int glyph_height;
    int width_in_glyphs;
    int height_in_glyphs;
    char first_character;
}
Font;

Font *load_bitmap_font(const char *filename, int glyph_width, 
    int glyph_height, int width_in_glyphs, int height_in_glyphs, char first_character)
{
    Font *font = malloc(sizeof(*font));

    font->texture = load_bitmap(filename);
    if (!font->texture) {
        free(font);
        return NULL;
    }

    font->glyph_width = glyph_width;
    font->glyph_height = glyph_height;
    font->width_in_glyphs = width_in_glyphs;
    font->height_in_glyphs = height_in_glyphs;
    font->first_character = first_character;

    return font;
}

Font *destroy_font(Font *font)
{
    font->texture = destroy_texture(font->texture);
    free(font);
    return NULL;
}

void draw_glyph(Font *font, char c, int x, int y)
{
    SDL_Rect source = { 0, 0, font->glyph_width, font->glyph_height };

    char index = c - font->first_character;

    if (index > 0) {
        source.y = index / font->width_in_glyphs;
        source.x = index - (font->width_in_glyphs * source.y);

        source.x *= font->glyph_width + 1;
        source.y *= font->glyph_height + 1;

        source.x += 1;
        source.y += 1;
    }

    SDL_Rect destination = { x, y, font->glyph_width, font->glyph_height };
    SDL_RenderCopy(sdl.renderer, font->texture->handle, &source, &destination);
}

void draw_string(Font *font, const char *string, int x, int y)
{
    int glyph_x = x;
    int glyph_y = y;
    int kerning = 1;

    while (*string) {
        draw_glyph(font, *string, glyph_x, glyph_y);
        glyph_x += font->glyph_width + kerning;

        ++string;
    }
}
