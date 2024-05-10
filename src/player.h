#include <sys/types.h>

struct Player {
    char *firstName;
    char *secondName;
    uint points;
};

struct Team {
    char *name;
    struct Player *players;
    size_t teamSize;
    uint totalPoints;
};

struct Match {
    struct Team *teamOne;
    struct Team *teamTwo;
};

void freePlayer(void *);
void freeTeam(void *);
