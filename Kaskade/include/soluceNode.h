#ifndef SOLUCENODE
#define SOLUCENODE

#include <state.h>
#include <stdbool.h>

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

int runThrough(struct state *state, struct done done, int depth);

int executeDestroy(struct state state, struct done *donen, int x, int y, int depth);

int destroyElem(char state[SIDE][SIDE], int x, int y, char del, struct done *done);

void fall(char state[SIDE][SIDE]);

#endif // SOLUCENODE

