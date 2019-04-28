typedef struct Font
{
    Texture *texture;
    int glyph_width;
    int glyph_height;
    int width_in_glyphs;
    int height_in_glyphs;
    char first_character;
    int kerning;
    int line_spacing;
    u32 color;
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
    font->kerning = 1;
    font->line_spacing = 3;
    font->color = 0xffffffff;

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

    u8 r, g, b;
    hex_to_argb(font->color, NULL, &r, &g, &b);
    SDL_SetTextureColorMod(font->texture->handle, r, g, b);

    SDL_RenderCopy(sdl.renderer, font->texture->handle, &source, &destination);
}

// Returns the y position of next line below the drawn string
int draw_string(Font *font, const char *string, int x, int y)
{
    int glyph_x = x;
    int glyph_y = y;

    while (*string) {
        if (*string == '\n') {
            glyph_y += font->glyph_height + font->line_spacing;
            glyph_x = x;
        }
        else if (*string == '\t') {
            glyph_x += font->glyph_width * 4;
        }
        else {
            draw_glyph(font, *string, glyph_x, glyph_y);
            glyph_x += font->glyph_width + font->kerning;
        }

        ++string;
    }

    return glyph_y + font->glyph_height + font->line_spacing;
}

int draw_wrapped_string(Font *font, const char *string, int x, int y, int max_width)
{
    strncpy(temporary_string, string, TEMPORARY_STRING_SIZE);
    temporary_string[TEMPORARY_STRING_SIZE - 1] = '\0';

    char *str = temporary_string;
    char *last_space = str;

    int width = 0;

    while (*str) {
        if (*str == ' ') {
            last_space = str;
        }

        width += font->glyph_width + font->kerning;

        if (width > max_width) {
            *last_space = '\n';
            width = 0;
        }

        ++str;
    }

    return draw_string(font, temporary_string, x, y);
}
