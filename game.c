Sprite *player = 0;
Sprite *sun = 0;
Font   *font = 0;

const char *right_key = "Right";
const char *left_key = "Left";
const char *up_key = "Up";
const char *down_key = "Down";

int food =  10;
int water = 50;
int money = 2;

bool setup_game()
{
    // Player
    player = load_sprite("assets/player.bmp");
    if (!player)
        return false;

    player->x = render_width / 2 - player->width / 2;
    player->y = render_height - player->height * 2;
    player->color = 0xffffffff;
    player->alpha = 0x88;

    // Sun
    sun = load_sprite("assets/sun.bmp");
    if (!sun)
        return false;

    sun->color = 0xffffff88;
    sun->x = 250; 
    sun->y = 50;

    // Font
    font = load_bitmap_font("assets/font.bmp", 6, 8, 16, 6, ' ');
    if (!font)
        return false;

    return true;
}

// Returns false when the program should end
bool update_game()
{
    // Sky
    set_hex_color(0xff4080ff);
    draw_rectangle(0, 0, render_width, render_height);

    // Sun
    draw_sprite(sun);

    // Floor
    set_hex_color(0xffffd8b0);
    draw_rectangle(0, render_height - (player->height + 10), render_width, player->height + 10);

    // Player
    draw_sprite(player);

    // Resources
    snprintf(temporary_string, TEMPORARY_STRING_SIZE, "Food: %i | Water: %i | Money: %i",
            food, water, money);
    draw_string(font, temporary_string, 2, 2);

    return true;
}

void destroy_game()
{
    // TODO
}
