Texture *texture;

bool setup_game()
{
    texture = load_bitmap("assets/foo.bmp");
    if (!texture)
        return false;

    return true;
}

// Returns false when the program should end
bool update_game()
{
    draw_texture(texture, 10, 10);

    return true;
}

void destroy_game()
{
    texture = destroy_texture(texture);
}
