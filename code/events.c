typedef struct Choice

{
    char    *label;
    char    *(*callback)();
}
Choice;

typedef struct Event
{
    char  *name;
    char  *label;
    Choice choice_a;
    Choice choice_b;
    bool   (*condition)();
    bool   allow_random_selection;
}
Event;

Event  *displaying_event = NULL;

bool event_conditions_fulfilled()
{
    if (!displaying_event->condition) {
        return true;
    }

    return displaying_event->condition();
}

char *show_event(Event *event)
{
    if (event == NULL)
        return NULL;

    displaying_event = event;
}

// TODO change these to snake case
char *cross_the_bridge()
{
    return "You walk over the bridge";
}

char *find_another_way()
{
    return "You find another way to cross the creek";
}

char *talk_to_priest()
{
    return "You talk to the priest";
}

char *ignore_priest()
{
    return "You ignore the priest";
}

bool check_for_bandages()
{
    return bandages > 0;
}

char *apply_bandages()
{
    --bandages;
    bleeding = false;
    return "You apply a bandage to the leg.";
}

char *ignore_wound()
{
    if (bleeding)
        died_of_bleeding = true;
    else 
        bleeding = true;

    return "You leave the wound untreated.";
}

char *bargain_bandage()
{
    return "You sell the stars in the sky for a healed wound.";
}

char *ignore_devil()
{
    return "You ignore the mysterious man.";
}

char *hear_out_eye_for_food()
{
    return "bargain_eye_for_food";
}

char *hear_out_stars_for_speed()
{
    return "bargain_stars_for_speed";
}

char *hear_out_color_for_health()
{
    return "bargain_colors_for_health";
}

#define EVENTS_SIZE 6
Event events[EVENTS_SIZE] = {
    {
        "creek",
        "You arrive at a creek. There is a poorly maintained bridge.",
        { "Cross the bridge",  cross_the_bridge, },
        { "Find another way",  find_another_way, },
        NULL,
        true,
    },
    {
        "priest",
        "You encounter a travelling priest.",
        { "Talk to him",  talk_to_priest, },
        { "Ignore him",  ignore_priest, },
        NULL,
        true,
    },
    {
        "minor_injury",
        "You cut your leg on a sharp rock.",
        { "Apply a bandage",  apply_bandages, },
        { "Ignore the wound",  ignore_wound, },
        check_for_bandages,
        true,
    },
    {
        "eye_for_food",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_eye_for_food, },
        { "Ignore him",  ignore_devil, },
        NULL,
        true,
    },
    {
        "stars_for_speed",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_stars_for_speed, },
        { "Ignore him",  ignore_devil, },
        NULL,
        true,
    },
    {
        "colors_for_health",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_color_for_health, },
        { "Ignore him",  ignore_devil, },
        NULL,
        true,
    },
};

Event *get_random_event()
{
    Event *event;

    // TODO make a shuffled list at the start and use that to track seen
    do {
        event = &events[rand() % EVENTS_SIZE];
    }
    while(!event->allow_random_selection);

    return event;
}

Event *get_event_by_name(const char *name)
{
    for (int i = 0; i < EVENTS_SIZE; ++i) {
        if (!strcmp(events[i].name, name)) {
            return &events[i];
        }
    }

    return NULL;
}

char *decline_devil()
{
    return "You decline and quick move along.";
}

char *remove_eye_for_food()
{
    food += 99999;
    left_eye_gone = true;
    return "You take the knife and cut your left eye from it's socket. The pain is intense, but you are surprised at the lack of bleeding. The man vanishes, leaving behind a plentiful sack of grain.";
}

char *remove_stars_for_speed()
{
    miles_per_hour *= 3;
    stars_gone = true;
    return "You look up and observe as all the stars in the sky vanish in an instant. When you direct your gaze back to the man, you find he is no where to be seen. However, your legs feel vital, as if they can walk much faster than previously.";
}

char *remove_color_for_health()
{
    bandages = 99999;
    color_gone = true;
    return "Suddenly, the world around you becomes devoid of saturation. Everything is a tint of black or a shade of white. The man disappears, leaving behind a pile of bandages.";
}

#define BARGAINS_SIZE 3
Event bargains[BARGAINS_SIZE] = {
    {
        "bargain_eye_for_food",
        "\"I have enough food here to keep you full for life. All I ask in return is a mere trifle... your left eye.\" The man draws a menacing knife from his pocket.",
        { "Take the blade and cut out your eye",  remove_eye_for_food, },
        { "Decline his ridiculous proposal",  decline_devil, },
        NULL,
        false,
    },
    {
        "bargain_stars_for_speed",
        "\"I can make your journey many hours shorter... all I require\" - He gestures upwards - \"is the stars in your sky.",
        { "Accept his offer",  remove_stars_for_speed, },
        { "Decline his bizarre proposal",  decline_devil, },
        NULL,
        false,
    },
    {
        "bargain_colors_for_health",
        "I can take the pain away. You'll never hurt again. All I need... is the color from your world.",
        { "Accept his offer",  remove_color_for_health, },
        { "Decline his disturbing proposal",  decline_devil, },
        NULL,
        false,
    },
};

Event *get_bargain_by_name(const char *name)
{
    for (int i = 0; i < BARGAINS_SIZE; ++i) {
        if (!strcmp(bargains[i].name, name)) {
            return &bargains[i];
        }
    }

    return NULL;
}
