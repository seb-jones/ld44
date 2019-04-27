typedef struct SkyColor
{
    int start_hour;
    int end_hour;
    u32 color;
}
SkyColor;

#define SKY_COLORS_SIZE 5
SkyColor sky_colors[SKY_COLORS_SIZE] = {
    { 6, 9, 0xfffa742d },
    { 9, 15, 0xff4080ff },
    { 15, 18, 0xff8a340d },
    { 18, 24, 0xff000044 },
    { 0, 6, 0xff000044 },
};

double  distance_left = 100.0;
double  miles_per_hour = 1.0;
double  minute_timer = 0;
int     day = 0;
int     hour = 12;
int     minute = 58;

Sprite *player = 0;
Sprite *sun = 0;
Sprite *devil = 0;
Font   *font = 0;

bool setup_game()
{
    // Font
    font = load_bitmap_font("assets/font.bmp", 6, 8, 16, 6, ' ');
    if (!font)
        return false;

    // Sun
    sun = load_sprite("assets/sun.bmp");
    if (!sun)
        return false;

    sun->color = 0xffffff88;
    sun->alpha = 0xdd;
    sun->x = 250; 
    sun->y = 50;

    // Player
    player = load_sprite("assets/player.bmp");
    if (!player)
        return false;

    player->x = render_width / 2 - player->width / 2;
    player->y = render_height - player->height * 2;

    // Devil
    devil = load_sprite("assets/devil.bmp");
    if (!devil)
        return false;

    devil->x = render_width - devil->width - 20;
    devil->y = render_height - devil->height * 2;
    devil->visible = false;

    return true;
}

#define ONE_SECOND 0.01

// Returns false when the program should end
bool update_game()
{
    if (displaying_event) {
        if (event_conditions_fulfilled() && key_just_down(a_key)) {
            displaying_event->choice_a.callback();
            displaying_event = NULL;
            devil->visible = false;
        }
        else if (key_just_down(b_key)) {
            displaying_event->choice_b.callback();
            displaying_event = NULL;
            devil->visible = false;
        }
        else if (!event_conditions_fulfilled() && key_just_down(c_key)) {
            show_event(get_bargain_by_name(displaying_event->bargain_name));
            devil->visible = true;
        }
    }
    else {
        minute_timer += elapsed_seconds;
        while (minute_timer >= ONE_SECOND) {
            ++minute;

            if (minute >= 60) {
                ++hour;

                show_event(&events[2]);

                distance_left -= miles_per_hour;

                if (hour >= 24) {
                    hour = 0;

                    ++day;
                }

                minute = 0;
            }
            minute_timer -= ONE_SECOND;
        }
    }

    // Sky
    {
        u32 sky_color = 0xffff00ff;
        for (int i = 0; i < SKY_COLORS_SIZE; ++i) {
            if (hour >= sky_colors[i].start_hour && hour < sky_colors[i].end_hour) {
                sky_color = sky_colors[i].color;
            }
        }

        set_hex_color(sky_color);
        draw_rectangle(0, 0, render_width, render_height);
    }

    // Sun
    draw_sprite(sun);

    // Floor
    set_hex_color(0xffffd8b0);
    draw_rectangle(0, render_height - (player->height + 10), render_width, player->height + 10);

    // Player
    draw_sprite(player);

    // Devil
    draw_sprite(devil);

    // Text
    snprintf(temporary_string, TEMPORARY_STRING_SIZE, "Day %i | Time %02i:%02i\n%.0f miles left to town", day, hour, minute, distance_left);
    draw_string(font, temporary_string, 2, 2);

    if (displaying_event) {
        snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                "%s Do you...", displaying_event->label);
        draw_wrapped_string(font, temporary_string, 20, 100, 
                render_width - 40);

        if (!event_conditions_fulfilled()) {
            font->color = 0xff888888;
        }

        snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                "a - %s", displaying_event->choice_a.label);

        draw_string(font, temporary_string, 20, 130);

        font->color = 0xffffffff;

        snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                "b - %s", displaying_event->choice_b.label);

        draw_string(font, temporary_string, 20, 145);

        if (!event_conditions_fulfilled()) {
            font->color = 0xffaa1111;
            snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                    "c - Bargain", displaying_event->choice_b.label);

            draw_string(font, temporary_string, 20, 160);
            font->color = 0xffffffff;
        }
    }

    return true;
}

void destroy_game()
{
    // TODO
}
