typedef struct Choice
{
    char *label;
    void (*callback)();
}
Choice;

typedef struct Event
{
    char *name;
    char *label;
    Choice choice_a;
    Choice choice_b;
}
Event;

void crossTheBridge()
{
    SDL_Log("You walk over the bridge");
}

void findAnotherWay()
{
    SDL_Log("You find another way to cross the creek");
}

void talkToPriest()
{
    SDL_Log("You talk to the priest");
}

void ignorePriest()
{
    SDL_Log("You ignore the priest");
}

#define EVENTS_SIZE 2
Event events[EVENTS_SIZE] = {
    {
        "creek",
        "You arrive at a creek. There is a poorly maintained bridge.",
        { "Cross the bridge",  crossTheBridge, },
        { "Find another way",  findAnotherWay, },
    },
    {
        "priest",
        "You encounter a travelling priest.",
        { "Talk to him",  talkToPriest, },
        { "Ignore him",  ignorePriest, },
    },
};
