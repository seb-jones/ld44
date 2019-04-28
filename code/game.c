Sprite  *player = NULL;
Sprite  *devil  = NULL;
Font    *font   = NULL;
Texture *left_eye_gone_overlay = NULL;
Texture *player_monochrome     = NULL;
Texture *devil_monochrome      = NULL;

char *displaying_outcome = "You hoist your bag onto you back, filled with what little food and supplies you have, and set out across the desert. You are sure you will find love and fortune on the other side, if you can only endure this trial...";

double  minute_timer = 0;
bool    endgame = false;
bool    hearing_proposal = false;
bool    bargaining = false;

bool time_equals(int _day, int _hour, int _minute)
{
    return day == _day && hour == _hour && minute == _minute;
}

bool setup_game()
{
    // Font
    font = load_bitmap_font("assets/font.bmp", 6, 8, 16, 6, ' ');
    if (!font)
        return false;

    if (!load_sky())
        return false;

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

    left_eye_gone_overlay = load_bitmap("assets/left_eye_gone.bmp");
    if (!left_eye_gone_overlay)
        return false;

    player_monochrome = load_bitmap("assets/player_monochrome.bmp");
    if (!player_monochrome)
        return false;

    devil_monochrome = load_bitmap("assets/devil_monochrome.bmp");
    if (!devil_monochrome)
        return false;

    return true;
}

#define ONE_SECOND 0.01

// Returns false when the program should end
bool update_game()
{
    if (died_of_starvation) {
        draw_wrapped_string(font, "Death by starvation text here", 20, 100, 
                render_width - 40);

        if (key_just_down(okay_key)) {
            return false;
        }

        return true;
    }

    if (died_of_bleeding) {
        draw_wrapped_string(font, "Death by bleeding text here", 20, 100, 
                render_width - 40);

        if (key_just_down(okay_key)) {
            return false;
        }

        return true;
    }

    if (endgame) {
        draw_wrapped_string(font, "Endgame text here", 20, 100, 
                render_width - 40);

        if (key_just_down(okay_key)) {
            return false;
        }

        return true;
    }

    if (displaying_event || displaying_outcome) {
        if (displaying_outcome) {
            if (key_just_down(okay_key)) {
                displaying_outcome = NULL;
                displaying_event = NULL;
                devil->visible = false;
            }
        }
        else {
            if (event_conditions_fulfilled() && key_just_down(a_key)) {
                if (bargaining) {
                    show_event(get_bargain_by_name(
                                displaying_event->choice_a.callback()));
                    bargaining = false;
                }
                else {
                    displaying_outcome = displaying_event->choice_a.callback();
                }
            }
            else if (key_just_down(b_key)) {
                displaying_outcome = displaying_event->choice_b.callback();
            }
        }
    }
    else {
        minute_timer += elapsed_seconds;
        while (minute_timer >= ONE_SECOND) {
            ++minute;

            if (minute >= 60) {
                ++hour;

                distance_left -= miles_per_hour;

                if (distance_left <= 0) {
                    endgame = true;
                    return true;
                }

                if (time_equals(1, 0, 60)) {
                    show_event(get_event_by_name("stars_for_health"));
                    bargaining = true;
                    devil->visible = true;
                }
                if (time_equals(2, 0, 60)) {
                    show_event(get_event_by_name("eye_for_food"));
                    bargaining = true;
                    devil->visible = true;
                }
                else if (time_equals(3, 0, 60)) {
                    show_event(get_event_by_name("colors_for_speed"));
                    bargaining = true;
                    devil->visible = true;
                }
                else if (hour % 6 == 0) {
                    show_event(get_random_event());
                }

                if (hour >= 24) {
                    hour = 0;

                    if (food > 0) {
                        --food;
                    }
                    else {
                        died_of_starvation = true;
                        return true;
                    }

                    ++day;
                }

                minute = 0;
            }
            minute_timer -= ONE_SECOND;
        }
    }

    if (color_gone) {
        player->texture = player_monochrome;
        devil->texture = devil_monochrome;
    }

    draw_sky();

    // Floor
    if (color_gone)
        set_hex_color(0xffdddddd);
    else 
        set_hex_color(0xffffd8b0);

    draw_rectangle(0, render_height - (player->height + 10), render_width, player->height + 10);

    // Player
    draw_sprite(player);

    // Devil
    draw_sprite(devil);

    // Left Eye Gone
    if (left_eye_gone)
        draw_texture(left_eye_gone_overlay, 0, 0);

    // Text
    snprintf(temporary_string, TEMPORARY_STRING_SIZE, "Day %i | Time %02i:%02i\n%.0f miles left to town\nFood - %i | Bandages - %i | Money - %i", day, hour, minute, distance_left, food, bandages, money);
    draw_string(font, temporary_string, 2, 2);

    if (displaying_outcome) {
        draw_wrapped_string(font, displaying_outcome, 20, 100, 
                render_width - 40);
    }
    else if (displaying_event) {
        draw_wrapped_string(font, displaying_event->label, 20, 100, 
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
    }

    return true;
}

void destroy_game()
{
    // TODO destroy assets
}
