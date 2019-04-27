typedef struct SkyColor
{
    int start_hour;
    int end_hour;
    u32 color;
}
SkyColor;

#define SKY_COLORS_SIZE 6
SkyColor sky_colors[SKY_COLORS_SIZE] = {
    { 6, 9, 0xfffa742d },
    { 9, 12, 0xff4080ff },
    { 12, 15, 0xff4080ff },
    { 15, 18, 0xff4080ff },
    { 18, 24, 0xff000044 },
    { 0, 6, 0xff000044 },
};

Sprite *sun = 0;

bool load_sky()
{
    // Sun
    sun = load_sprite("assets/sun.bmp");
    if (!sun)
        return false;

    sun->color = 0xffffff88;
    sun->alpha = 0xdd;
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

    double t = (double)(hour - sky_color->start_hour) /
        (double)(next_sky_color->start_hour - sky_color->start_hour);

    set_hex_color(lerp_color(sky_color->color, next_sky_color->color, t));
    draw_rectangle(0, 0, render_width, render_height);

    if (hour >= sunrise && hour <= sunset) {
        SDL_Log("%f", inverse_lerp_int(sunrise, sunset, hour));
        sun->x = (int)(inverse_lerp_int(sunrise, sunset, hour) *
                (double)(((render_width * 2)) / 24)) * sun->width;
    }

    // Sun
    draw_sprite(sun);
}
