#ifndef STATE
#define STATE

#include <stddef.h>

#define SIDE 8

struct state {
  char element[SIDE][SIDE];
  int score;
  int destroyed;
};

struct done {
  char element[SIDE][SIDE];
};

static const char emptyToken = '-';

void initState(struct state *state);

void initdone(struct done *done);

int getScoreElementDestroyed(int destroyed);

void printState(struct state *state, struct done *before, struct done *done);

#endif // STATE
