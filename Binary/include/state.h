#ifndef STATE
#define STATE

#include <stddef.h>

#define SIDE 5

static const int stateSize = SIDE*SIDE;
struct state {
  char remainingAttempt;
  char element[SIDE][SIDE];
};

struct soluceNode;

struct nodeList {
  int x, y;
  struct nodeList *next;
};

struct queue {
  int size;
  struct nodeList *head;
  struct nodeList *tail;
};

void initQueue(struct queue *queue);

void addToQueue(struct queue *queue, int x, int y);

struct nodeList * pick(struct queue *queue);

static const char emptyToken = '-';

void initState(struct state *state);

int compareState(struct state *state1, struct state *state2);

int computeHeuristique(struct state *state);

int isAttemptValid(struct state *state, int x, int y);

int executeAttempt(struct state state, int x, int y, int depth, int attemptAhead, struct soluceNode *tree, int ATTEMPT_TODO[3][2]);

void moveElements(struct state *state, int x, int y);

int destroyFallLoop(struct state *state, int attemptAhead);

int destroyElem(char state[SIDE][SIDE], int x, int y);

void fall(char state[SIDE][SIDE]);

void printState(struct state *state, int x, int y);

#endif // STATE
