Texture *texture;
Font    *font;

bool setup_game()
{
    texture = load_bitmap("assets/foo.bmp");
    if (!texture)
        return false;

    font = load_bitmap_font("assets/font.bmp", 6, 8, 16, 6, ' ');
    if (!font)
        return false;

    return true;
}

// Returns false when the program should end
bool update_game()
{
    //draw_texture(font->texture, 10, 10);

    draw_string(font, "Hello, World!", 50, 50);

    return true;
}

void destroy_game()
{
    texture = destroy_texture(texture);
}
