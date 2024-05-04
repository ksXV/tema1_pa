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
};

void freePlayer(struct Player *);
void freeTeam(struct Team *);
