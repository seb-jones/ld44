// System
double elapsed_seconds;

const char *one_key = "1";
const char *two_key = "2";
const char *okay_key = "Return";
const char *fullscreen_key = "F11";

// Gameplay
#define BASE_MILES_PER_HOUR 1.0

double  distance_left = 90;
double  miles_per_hour = BASE_MILES_PER_HOUR;
int     day = 1;
int     hour = 5;
int     minute = 50;
int     sunrise = 6;
int     sunset  = 18;
int     food = 1;
int     bandages = 1;
int     money = 5;
bool    left_eye_gone = false;
bool    stars_gone = false;
bool    color_gone = false;
bool    bleeding = false;
bool    died_of_starvation = false;
bool    died_of_bleeding = false;
