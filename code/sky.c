typedef struct SkyColor
{
    int start_hour;
    int end_hour;
    u32 color;
}
SkyColor;

#define SKY_COLORS_SIZE 7
SkyColor sky_colors[SKY_COLORS_SIZE] = {
    { 6, 9, 0xff000044 },
    { 9, 12, 0xff4080ff },
    { 12, 15, 0xff4080ff },
    { 15, 18, 0xff4080ff },
    { 18, 0, 0xff000044 },
    { 0, 6, 0xff000044 },
};

#define STARS_SIZE 12
SDL_Rect stars[STARS_SIZE] = {
    { 10, 10, 1, 1 },
    { 100, 50, 1, 1 },
    { 60, 20, 1, 1 },
    { 30, 70, 1, 1 },
    { 100, 100, 1, 1 },
    { 200, 80, 1, 1 },
    { 230, 170, 1, 1 },
    { 180, 120, 1, 1 },
    { 120, 160, 1, 1 },
    { 80, 150, 1, 1 },
    { 300, 30, 1, 1 },
    { 280, 100, 1, 1 },
};

Sprite *sun = 0;

bool load_sky()
{
    // Sun
    sun = load_sprite("assets/sun.bmp");
    if (!sun)
        return false;

    sun->color = 0xffffffff;
    sun->alpha = 0xff;
    sun->x = 250; 
    sun->y = 50;

    return true;
}

void draw_sky()
{
    SkyColor *sky_color = &sky_colors[0];
    SkyColor *next_sky_color = &sky_colors[0];

    for (int i = 0; i < SKY_COLORS_SIZE; ++i) {
        if (hour >= sky_colors[i].start_hour && hour < sky_colors[i].end_hour) {
            sky_color = &sky_colors[i];

            if (i == SKY_COLORS_SIZE - 1)
                next_sky_color = &sky_colors[0];
            else
                next_sky_color = &sky_colors[i + 1];
        }
    }


    int start_minutes = sky_color->start_hour * 60;
    int end_minutes   = next_sky_color->start_hour * 60;
    int current_minutes = hour * 60 + minute;

    if (hour >= sunrise && hour <= sunset) {
        double t = inverse_lerp_int(sunrise * 60, sunset * 60, (hour * 60) + minute);
        sun->x = (int)(t * (double)(render_width * 2)) - (render_width / 2);
        sun->y = render_height - 200 + 
           (cos(t * (2.0 * M_PI)) * (double)sun->height * 1.5); 
    }

    // Sky Background
    if (color_gone) {
        set_hex_color(0xffdddddd);
    }
    else {
        double t = inverse_lerp_int(start_minutes, end_minutes, current_minutes);
        set_hex_color(lerp_color(sky_color->color, next_sky_color->color, t));
    }

    draw_rectangle(0, 0, render_width, render_height);

    // Stars
    if (!stars_gone && (hour >= sunset || hour <= sunrise)) {
        set_hex_color(0xffffffff);
        for (int i = 0; i < STARS_SIZE; ++i)
            SDL_RenderFillRect(sdl.renderer, &stars[i]);
    }

    // Sun
    if (hour > sunrise && hour < sunset)
        draw_sprite(sun);
}
