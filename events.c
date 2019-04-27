typedef struct Choice
{
    char    *label;
    char * (*callback)();
}
Choice;

typedef struct Event
{
    char  *name;
    char  *label;
    Choice choice_a;
    Choice choice_b;
    bool (*condition)();
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
char *crossTheBridge()
{
    return "You walk over the bridge";
}

char *findAnotherWay()
{
    return "You find another way to cross the creek";
}

char *talkToPriest()
{
    return "You talk to the priest";
}

char *ignorePriest()
{
    return "You ignore the priest";
}

bool checkForBandage()
{
    return bandages > 0;
}

char *applyBandage()
{
    --bandages;
    return "You apply a bandage to the leg.";
}

char *ignoreWound()
{
    return "You leave the wound untreated.";
}

char *bargainBandage()
{
    return "You sell the stars in the sky for a healed wound.";
}

#define EVENTS_SIZE 3
Event events[EVENTS_SIZE] = {
    {
        "creek",
        "You arrive at a creek. There is a poorly maintained bridge.",
        { "Cross the bridge",  crossTheBridge, },
        { "Find another way",  findAnotherWay, },
        NULL,
        NULL,
    },
    {
        "priest",
        "You encounter a travelling priest.",
        { "Talk to him",  talkToPriest, },
        { "Ignore him",  ignorePriest, },
        NULL,
        NULL,
    },
    {
        "minor_injury",
        "You cut your leg on a sharp rock.",
        { "Apply a bandage",  applyBandage, },
        { "Ignore the wound",  ignoreWound, },
        checkForBandage,
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
        { "Let him heal the wound",  bargainBandage, },
        { "Ignore him",  ignoreWound, },
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
