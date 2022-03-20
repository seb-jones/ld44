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

void show_event(Event *event)
{
	if (event == NULL)
		return;

    displaying_event = event;
	displaying_event->seen = true;
}

char *ford_the_river()
{
    bandages = 0;
    distance_left -= 5;
    return "When you reach the other side of the river, you find that your medical supplies are soaked and will no longer be usable.";
}

char *find_another_way_across_the_river()
{
    return "You find another way to cross the river.";
}

char *help_soldier()
{
    --bandages;
    ++food;
    return "The soldier is thankful, and shares some of his food rations with you.";
}

char *ignore_soldier()
{
    return "You ignore the soldier.";
}

char *feed_priest()
{
    --food;
    distance_left -= 5;
    return "You give the priest some food. He is gratious and tells you of a shortcut that he knows.";
}

char *ignore_priest()
{
    return "You ignore the priest.";
}

bool check_for_bandages()
{
    return bandages > 0;
}

bool check_for_food()
{
    return food > 0;
}

bool check_for_5_money()
{
    return money >= 5;
}

bool can_pay_doctor()
{
    return money >= 5 && (bleeding || miles_per_hour < BASE_MILES_PER_HOUR);
}

char *apply_bandages()
{
    --bandages;

    if (miles_per_hour < BASE_MILES_PER_HOUR) {
        miles_per_hour = BASE_MILES_PER_HOUR;
    }

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

char *ignore_trap_wound()
{
    miles_per_hour = 0.5;
    ignore_wound();
    return "The untreated leg makes it harder to walk";
}

char *accept_merchant()
{
    money -= 5;
    ++food;
    return "You trade 5 coins for a piece of food.";
}

char *decline_merchant()
{
    return "You politely decline.";
}

char *accept_doctor()
{
    money -= 5;
    bleeding = false;

    if (miles_per_hour < BASE_MILES_PER_HOUR) 
        miles_per_hour = BASE_MILES_PER_HOUR;

    return "You trade 5 coins for the Doctor's services.";
}

char *investigate_vultures()
{
    ++food;
    distance_left += 5;
    return "You find a recently-dead animal that is in edible condition. However, the detour ends up adding distance to your journey.";
}

char *ignore_vultures()
{
    return "You ignore the vultures and move along.";
}

char *eat_mushrooms()
{
    ++food;
    bleeding = true;
    return "The mushrooms satisfy your hunger, but make you feel ill.";
}

char *ignore_mushrooms()
{
    return "You leave the mushrooms as they are.";
}

char *decline_doctor()
{
    return "You politely decline.";
}

char *fight_bandits()
{
    money = 0;
    ignore_wound();
    return "The bandits give you a severe beating and take your money.";
}

char *evade_bandits()
{
    distance_left += 10;
    return "You manage to evade the bandits, but it adds distance to your journey.";
}

char *take_food()
{
    ++food;
    return "You take the food.";
}

char *leave_food()
{
    return "You leave the food where it is.";
}

char *take_bandages()
{
    ++bandages;
    return "You take the bandages.";
}

char *leave_bandages()
{
    return "You leave the bandages where they are.";
}

char *take_money()
{
    money += 5;
    return "You take the money.";
}

char *leave_money()
{
    return "You leave the money where it is.";
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

char *hear_out_stars_for_health()
{
    return "bargain_stars_for_health";
}

char *hear_out_color_for_speed()
{
    return "bargain_colors_for_speed";
}

#define EVENTS_SIZE 15
Event events[EVENTS_SIZE] = {
    {
        "river",
        "You arrive at a river. It is running fast, but fording it would provide a considerable saving of time.",
        { "Ford the river",  ford_the_river, },
        { "Find another way",  find_another_way_across_the_river, },
        NULL,
        true,
    },
    {
        "priest",
        "You encounter a travelling priest. He looks hungry.",
        { "Give him some food",  feed_priest, },
        { "Ignore him",  ignore_priest, },
        check_for_food,
        true,
    },
    {
        "soldier",
        "You find a gravely-wounded soldier lying in the shade.",
        { "Bandage his wound",  help_soldier, },
        { "Ignore him",  ignore_soldier, },
        check_for_bandages,
        true,
    },
    {
        "trap",
        "You get your leg caught in a trap.",
        { "Apply a bandage",  apply_bandages, },
        { "Ignore the wound",  ignore_trap_wound, },
        check_for_bandages,
        true,
    },
    {
        "merchant",
        "A travelling merchant offers you some food for 5 coins.",
        { "Accept",  accept_merchant, },
        { "Decline",  decline_merchant, },
        check_for_5_money,
        true,
    },
    {
        "doctor",
        "You meet a doctor on his way to town. He offers to cure all your ills for 5 coins.",
        { "Accept",  accept_doctor, },
        { "Decline",  decline_doctor, },
        can_pay_doctor,
        true,
    },
    {
        "vultures",
        "You spot some vultures on the horizon.",
        { "Investigate",  investigate_vultures, },
        { "Ignore Them",  ignore_vultures, },
        NULL,
        true,
    },
    {
        "mushrooms",
        "You find some curious mushrooms.",
        { "Eat them",  eat_mushrooms, },
        { "Ignore Them",  ignore_mushrooms, },
        NULL,
        true,
    },
    {
        "bandits",
        "A group of bandits approach.",
        { "Stand your ground",  fight_bandits, },
        { "Evade them",  evade_bandits, },
        NULL,
        true,
    },
    {
        "food",
        "You find a can of food.",
        { "Take it.", take_food },
        { "Leave it.", leave_food },
        NULL,
        true,
    },
    {
        "bandages",
        "You find some bandages.",
        { "Take them.", take_bandages },
        { "Leave them.", leave_bandages },
        NULL,
        true,
    },
    {
        "money",
        "You find some money.",
        { "Take it.", take_money },
        { "Leave it.", leave_money },
        NULL,
        true,
    },

    {
        "eye_for_food",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_eye_for_food, },
        { "Ignore him",  ignore_devil, },
        NULL,
        false,
    },
    {
        "stars_for_health",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_stars_for_health, },
        { "Ignore him",  ignore_devil, },
        NULL,
        false,
    },
    {
        "colors_for_speed",
        "A mysterious man appears. He wears a fine suit and offers a proposal.",
        { "Hear him out",  hear_out_color_for_speed, },
        { "Ignore him",  ignore_devil, },
        NULL,
        false,
    },
};

Event *get_random_event()
{
    bool all_seen = true;
    for (int i = 0; i < EVENTS_SIZE; ++i) {
        if (!events[i].seen) {
            all_seen = false;
        }
    }

    if (all_seen) {
        for (int i = 0; i < EVENTS_SIZE; ++i) {
            events[i].seen = false;
        }
    }

    Event *event;

    do {
        event = &events[rand() % EVENTS_SIZE];
    }
    while(!event->allow_random_selection || event->seen);

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

char *remove_stars_for_health()
{
    stars_gone = true;
    bandages = 99999;
    bleeding = false;
    if (miles_per_hour < BASE_MILES_PER_HOUR) 
        miles_per_hour = BASE_MILES_PER_HOUR;

    return "You look up and observe as all the stars in the sky vanish in an instant. When you direct your gaze back to the man, you find nothing but a pile of bandages.";
}

char *remove_color_for_speed()
{
    miles_per_hour *= 3;
    color_gone = true;
    return "Suddenly, the world around you becomes devoid of saturation. Everything is a tint of black or a shade of white. You know not where the man has gone, but your legs feel vital and capable of superior speed.";
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
        "bargain_stars_for_health",
        "\"I can take the pain away. You'll never hurt again. All I require\" - He gestures upwards - \"is the stars in your sky.",
        { "Accept his offer",  remove_stars_for_health, },
        { "Decline his bizarre proposal",  decline_devil, },
        NULL,
        false,
    },
    {
        "bargain_colors_for_speed",
        "\"I can make your journey many hours shorter. All I need... is the colour of your world.\"",
        { "Accept his offer",  remove_color_for_speed, },
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
