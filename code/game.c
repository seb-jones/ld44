Sprite  *player = NULL;
Sprite  *devil  = NULL;
Sprite  *blood  = NULL;
Font    *font   = NULL;
Texture *left_eye_gone_overlay       = NULL;
Texture *player_monochrome           = NULL;
Texture *player_reg_frame            = NULL;
Texture *player_alt_frame            = NULL;
Texture *player_alt_frame_monochrome = NULL;
Texture *devil_monochrome            = NULL;

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

    player_reg_frame = player->texture;

    // Devil
    devil = load_sprite("assets/devil.bmp");
    if (!devil)
        return false;

    devil->x = render_width - devil->width - 20;
    devil->y = render_height - devil->height * 2;
    devil->visible = false;

    blood = load_sprite("assets/blood.bmp");
    if (!blood)
        return false;

    blood->color = 0xffff0000;
    blood->vel_x = -1;
    blood->visible = false;

    left_eye_gone_overlay = load_bitmap("assets/left_eye_gone.bmp");
    if (!left_eye_gone_overlay)
        return false;

    player_alt_frame = load_bitmap("assets/player_alt_frame.bmp");
    if (!player_alt_frame)
        return false;

    player_alt_frame_monochrome = load_bitmap(
            "assets/player_alt_frame_monochrome.bmp");
    if (!player_alt_frame_monochrome)
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
        draw_wrapped_string(font, "You have starved to death.", 8, 8, 
                render_width - 16);

        if (key_just_down(okay_key)) {
            return false;
        }

        return true;
    }

    if (died_of_bleeding) {
        draw_wrapped_string(font, "You have bled to death.", 8, 8, 
                render_width - 16);

        if (key_just_down(okay_key)) {
            return false;
        }

        return true;
    }

    if (endgame) {
        int next_y = 8;

        if (stars_gone) {
            next_y = draw_wrapped_string(font, 
                    "The stars in the sky no longer shine.", 8, 
                    next_y, render_width - 16);
            next_y += font->glyph_height;
        }

        if (left_eye_gone) {
            next_y = draw_wrapped_string(font, 
                    "Your left eye no longer serves a purpose.", 8, 
                    next_y, render_width - 16);
            next_y += font->glyph_height;
        }

        if (color_gone) {
            next_y = draw_wrapped_string(font, 
                    "The world is no longer adourned with colour.", 8, 
                    next_y, render_width - 16);
            next_y += font->glyph_height;
        }

        if (stars_gone || left_eye_gone || color_gone) {
            next_y += font->glyph_height * 2;

            next_y = draw_wrapped_string(font, 
                    "You still possess existence, but perhaps you have sold part of your life in order to maintain that possession...",
                    8, next_y, render_width - 16);
            next_y += font->glyph_height * 3;
        }
        else {
            next_y = draw_wrapped_string(font, 
                    "You made it intact. Perhaps here you will find a new beginning.", 8, 
                    next_y, render_width - 16);
            next_y += font->glyph_height * 3;

            next_y = draw_wrapped_string(font, 
                    "Perhaps every day is a new beginning...", 8, 
                    next_y, render_width - 16);
        }

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
            if (event_conditions_fulfilled() && key_just_down(one_key)) {
                if (bargaining) {
                    show_event(get_bargain_by_name(
                                displaying_event->choice_a.callback()));
                    bargaining = false;
                }
                else {
                    displaying_outcome = displaying_event->choice_a.callback();
                }
            }
            else if (key_just_down(two_key)) {
                displaying_outcome = displaying_event->choice_b.callback();
                bargaining = false;
            }
        }
    }
    else {
        update_sprite(blood);

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

                if (time_equals(1, 24, 60)) {
                    show_event(get_event_by_name("stars_for_health"));
                    bargaining = true;
                    devil->visible = true;
                }
                else if (time_equals(2, 24, 60)) {
                    show_event(get_event_by_name("eye_for_food"));
                    bargaining = true;
                    devil->visible = true;
                }
                else if (time_equals(3, 24, 60)) {
                    show_event(get_event_by_name("colors_for_speed"));
                    bargaining = true;
                    devil->visible = true;
                }
                else if (hour % 6 == 0) {
                    show_event(get_random_event());
                }

                if (bleeding && hour % 5 == 0) {
                    blood->x = player->x;
                    blood->y = player->y + player->height - blood->height;
                    blood->visible = true;
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
        if (hour % 2)
            player->texture = player_alt_frame_monochrome;
        else
            player->texture = player_monochrome;


        devil->texture = devil_monochrome;
    }
    else {
        if (hour % 2)
            player->texture = player_alt_frame;
        else
            player->texture = player_reg_frame;
    }

    draw_sky();

    // Floor
    if (color_gone)
        set_hex_color(0xff888888);
    else 
        set_hex_color(0xffffd8b0);

    draw_rectangle(0, render_height - (player->height + 10), render_width, player->height + 10);

    // Blood
    if (color_gone) {
        blood->color = 0xff333333;
    }

    draw_sprite(blood);

    // Player
    draw_sprite(player);

    // Devil
    draw_sprite(devil);

    // Left Eye Gone
    if (left_eye_gone)
        draw_texture(left_eye_gone_overlay, 0, 0);

    // Text
    snprintf(temporary_string, TEMPORARY_STRING_SIZE, "Day %i | Time %02i:%02i\n%.0f miles left\nFood - %i | Bandages - %i | Money - %i", day, hour, minute, distance_left, food, bandages, money);

    int next_y = draw_string(font, temporary_string, 2, 2);
    next_y += font->glyph_height * 2;


    if (displaying_outcome) {
        next_y = draw_wrapped_string(font, displaying_outcome, 20, next_y, 
                render_width - 40);
    }
    else if (displaying_event) {
        next_y = draw_wrapped_string(font, displaying_event->label, 20, next_y, 
                render_width - 40);
        next_y += font->glyph_height;

        if (!event_conditions_fulfilled()) {
            font->color = 0xff888888;
        }

        snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                "1 - %s", displaying_event->choice_a.label);

        next_y = draw_string(font, temporary_string, 20, next_y);
        next_y += font->glyph_height;

        font->color = 0xffffffff;

        snprintf(temporary_string, TEMPORARY_STRING_SIZE,
                "2 - %s", displaying_event->choice_b.label);

        draw_string(font, temporary_string, 20, next_y);
    }

    return true;
}

void destroy_game()
{
    // TODO destroy assets
}
