typedef struct InputKey
{
    bool down;
    bool previous;
}
InputKey;

typedef struct Input
{
    InputKey *keys;
    int       keys_count;
    const u8 *keyboard_state;
}
Input;

Input input = {0};

bool setup_input()
{
    input.keyboard_state = SDL_GetKeyboardState(&input.keys_count); 
    input.keys = malloc(input.keys_count * sizeof(*input.keys));
    memset(input.keys, 0, input.keys_count * sizeof(*input.keys));

    return true;
}

void update_input()
{
    for (int i = 0; i < input.keys_count; ++i) {
        input.keys[i].previous = input.keys[i].down;
        input.keys[i].down = input.keyboard_state[i];
    }
}

bool key_down(const char *keyname) 
{
    int i = SDL_GetScancodeFromName(keyname);
    return input.keys[i].down;
}

bool key_up(const char *keyname) 
{
    int i = SDL_GetScancodeFromName(keyname);
    return !input.keys[i].down;
}

bool key_just_down(const char *keyname) 
{
    int i = SDL_GetScancodeFromName(keyname);
    return input.keys[i].down && !input.keys[i].previous;
}

bool key_just_up(const char *keyname) 
{
    int i = SDL_GetScancodeFromName(keyname);
    return !input.keys[i].down && input.keys[i].previous;
}
