Texture *texture;
Font    *font;

const char *right_key = "Right";
const char *left_key = "Left";
const char *up_key = "Up";
const char *down_key = "Down";

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
    if (key_down(left_key)) {
        SDL_Log("LEFT DOWN");
    }

    if (key_just_up(up_key)) {
        SDL_Log("UP JUST UP");
    }

    if (key_just_down(right_key)) {
        SDL_Log("RIGHT JUST DOWN");
    }

    draw_string(font, "Hello, World!", 50, 50);

    return true;
}

void destroy_game()
{
    texture = destroy_texture(texture);
}
