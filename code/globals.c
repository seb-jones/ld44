// System
double elapsed_seconds;

const char *a_key = "A";
const char *b_key = "B";
const char *c_key = "C";
const char *okay_key = "Return";
const char *fullscreen_key = "F11";

// Gameplay
#define BASE_MILES_PER_HOUR 1.0

double  distance_left = 100;
double  miles_per_hour = BASE_MILES_PER_HOUR;
int     day = 1;
int     hour = 0;
int     minute = 50;
int     sunrise = 8;
int     sunset  = 17;
int     food = 0;
int     bandages = 1;
int     money = 10;
bool    left_eye_gone = false;
bool    stars_gone = false;
bool    color_gone = false;
bool    bleeding = false;
bool    died_of_starvation = false;
bool    died_of_bleeding = false;
