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
    char  *bargain_name;
    bool   seen;
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
    displaying_event->seen = true;
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
    return "You apply a bandage to the leg.";
}

char *ignore_wound()
{
    return "You leave the wound untreated.";
}

char *bargain_bandage()
{
    return "You sell the stars in the sky for a healed wound.";
}

#define EVENTS_SIZE 3
Event events[EVENTS_SIZE] = {
    {
        "creek",
        "You arrive at a creek. There is a poorly maintained bridge.",
        { "Cross the bridge",  cross_the_bridge, },
        { "Find another way",  find_another_way, },
        NULL,
        NULL,
    },
    {
        "priest",
        "You encounter a travelling priest.",
        { "Talk to him",  talk_to_priest, },
        { "Ignore him",  ignore_priest, },
        NULL,
        NULL,
    },
    {
        "minor_injury",
        "You cut your leg on a sharp rock.",
        { "Apply a bandage",  apply_bandages, },
        { "Ignore the wound",  ignore_wound, },
        check_for_bandages,
        "bargain_bandage"
    },
};

Event *get_random_event()
{
    bool all_events_seen = true; 
    for (int i = 0; i < EVENTS_SIZE; ++i) {
        if (!events[i].seen) {
            all_events_seen = false;
            break;
        }
    }

    if (all_events_seen) {
        SDL_Log("All events seen.");
        return NULL;
    }

    Event *event;

    // TODO make a shuffled list at the start and use that to track seen
    do {
        event = &events[rand() % EVENTS_SIZE];
    }
    while(event->seen);

    return event;
}

#define BARGAINS_SIZE 1
Event bargains[BARGAINS_SIZE] = {
    {
        "bargain_bandage",
        "I can heal these wounds... all I ask in return is a mere trifle... the stars in your sky.",
        { "Let him heal the wound",  bargain_bandage, },
        { "Ignore him",  ignore_wound, },
        NULL,
        NULL,
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
