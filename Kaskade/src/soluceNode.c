#include <soluceNode.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void initQueue(struct queue *queue) {
  queue->size = 0;
  queue->head = NULL;
  queue->tail = NULL;
}

void addToQueue(struct queue *queue, int x, int y) {
  struct nodeList *elem = malloc(sizeof(struct nodeList));
  elem->next = NULL;
  elem->x = x;
  elem->y = y;

  if(queue->tail) {
    queue->tail->next = elem;
  } else {
    queue->head = elem;
  }
  queue->tail = elem;
  queue->size++;
}

struct nodeList * pick(struct queue *queue) {
  if(!queue->head) {
    return NULL;
  }

  struct nodeList *head = queue->head;
  if(head->next) {
    queue->head = head->next;
  } else {
    queue->head = NULL;
    queue->tail = NULL;
  }
  queue->size--;
  return head;
};

static double max = 0;
int runThrough(struct state *state, struct done done, int depth) {
  if(depth > 2) {
    return 0;
  }
  int i, j, res = 0;
  for(i=0; i<SIDE; i++) {
    for(j=0; j<SIDE; j++) {
      if(state->element[i][j] == emptyToken || done.element[i][j]) {
        continue;
      }
      struct done before = done;
      if(executeDestroy(*state, &done, i, j, depth)) {
        char fill[depth+1];
        memset(fill, ' ', depth);
        fill[depth] = 0;
        printf("%s%d %d\n",fill, i, j);
        printState(state, &before, &done);
        res=1;
      }
    }
  }
  return res;
}

int executeDestroy(struct state state, struct done *done, int x, int y, int depth) {
  int res = 0;
  int nDestroyed = destroyElem(state.element, x, y, state.element[x][y], done);
  if(nDestroyed > 1) {
    state.destroyed += nDestroyed;
    state.score += getScoreElementDestroyed(nDestroyed);
    double ratio = (double)state.score/(depth+1);
    if(ratio > max) {
      printf("MAX => total: %d\tdestroyed: %d\tcoups: %d\t(%0.1f)\n",state.score, state.destroyed, depth+1, (double)state.score/(depth+1));
      max = ratio;
      res = 1;
    }
    return runThrough(&state, *done, depth+1) || res;
  }
  return 0;
}

int destroyElem(char state[SIDE][SIDE], int x, int y, char del, struct done *done) {
  struct queue queue;
  initQueue(&queue);

  addToQueue(&queue, x, y);

  int i, res = 1;
  state[x][y] = emptyToken;
  int nextX[] = { -1, 0, 1, 0 };
  int nextY[] = { 0, -1, 0, 1 };

  struct nodeList *next;
  while((next = pick(&queue)) != NULL) {
    //if(queue.size%10==0)printf("%d\n",queue.size);
    for(i=0; i<4; i++) {
      int nx = next->x+nextX[i], ny = next->y+nextY[i];
      if(nx >= 0 && nx < SIDE && ny >= 0 && ny < SIDE && state[nx][ny] == del) {
        res++;
        state[nx][ny] = emptyToken;
        done->element[nx][ny] = 1;
        addToQueue(&queue, nx, ny);
      }
    }
    free(next);
  }

  if(res == 1) {
    state[x][y] = del;
    return 0;
  }

  done->element[x][y] = 1;
  fall(state);

  return res;
}

void fall(char state[SIDE][SIDE]) {
  int i, j;
  for(j=0; j<SIDE; j++) {
    int decal = 0;
    for(i=SIDE-1; i>=0; i--) {
      if(state[i][j] == emptyToken) {
        decal++;
      } else {
        state[i+decal][j] = state[i][j];
      }
    }
    for(i=0; i<decal; i++) {
      state[i][j] = emptyToken;
    }
  }
  for(i=0; i<SIDE; i++) {
    int decal = 0;
    for(j=0; j<SIDE; j++) {
      if(state[i][j] == emptyToken) {
        decal++;
      } else {
        state[i][j-decal] = state[i][j];
      }
    }
    for(j=SIDE-decal; j<SIDE; j++) {
      state[i][j] = emptyToken;
    }
  }
}

/* crash 177
int destroyElem(char state[SIDE][SIDE], int x, int y, char del) {
  state[x][y] = emptyToken;
  int res = 1;
  if(x-1 >= 0 && state[x-1][y] == del) {
    res += destroyElem(state, x-1, y, del);
  }
  if(x+1 < SIDE && state[x+1][y] == del) {
    res += destroyElem(state, x+1, y, del);
  }
  if(y-1 >= 0 && state[x][y-1] == del) {
    res += destroyElem(state, x, y-1, del);
  }
  if(y+1 < SIDE && state[x][y+1] == del) {
    res += destroyElem(state, x, y+1, del);
  }
  return res;
}
*/
/* crash 204
int destroyElem(char state[SIDE][SIDE], int x, int y, char del) {
  if(x < 0 || x == SIDE || y < 0 || y == SIDE || state[x][y] != del) {
    return 0;
  }

  state[x][y] = emptyToken;

  return 1+
  destroyElem(state, x-1, y, del)+
  destroyElem(state, x+1, y, del)+
  destroyElem(state, x, y-1, del)+
  destroyElem(state, x, y+1, del);
}
*/
