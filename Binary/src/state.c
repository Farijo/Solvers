#include <state.h>
#include <stdio.h>
#include <soluceNode.h>
#include <windows.h>

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

void initState(struct state *state) {
  memset(state->element, '-', SIDE*SIDE);
  state->remainingAttempt = 3;
}

int compareState(struct state *state1, struct state *state2) {
  return memcmp(state1->element, state2->element, stateSize);
}

int manhathanDist(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

int toX(int t) {
  return t % SIDE;
}

int toY(int t) {
  return t / SIDE;
}

int computeHeuristique(struct state *state) {
  int values[SIDE][SIDE], i, j, MAX = SIDE*SIDE;
  memset(values, 0, SIDE*SIDE*sizeof(int));
  for(i=0; i<MAX; i++) {
    int x1 = toX(i), y1 = toY(i);
    char v = state->element[x1][y1];
    if(v == emptyToken) continue;
    for(j=i+1; j<MAX; j++) {
      int x2 = toX(j), y2 = toY(j);
      if(state->element[x2][y2] == v) {
        int dist = (SIDE-1)*2 - manhathanDist(x1, y1, x2, y2);
        values[x1][y1] += dist;
        values[x2][y2] += dist;
      }
    }
  }

  int res = 0;
  for(i=0; i<SIDE; i++) {
    for(j=0; j<SIDE; j++) {
      res += values[i][j];
    }
  }
  return res;
}

int isAttemptValid(struct state *state, int x, int y) {
  return state->element[x][y] != emptyToken
  && state->element[x+1][y] != emptyToken
  && state->element[x+1][y+1] != emptyToken
  && state->element[x][y+1] != emptyToken;
}

static int max = 0, maxHeuristique = 0;
int executeAttempt(struct state state, int x, int y, int depth, int attemptAhead, struct soluceNode *tree, int ATTEMPT_TODO[3][2]) {
  int nextAtt, ret = 0;

  moveElements(&state, x, y);

  nextAtt = destroyFallLoop(&state, attemptAhead);

  if(state.remainingAttempt > 0) {
      if(nextAtt > max) {
        max = nextAtt;
        maxHeuristique = computeHeuristique(&state);
        ret = 1;
        printf("HEURISTIQUE: %d\n",maxHeuristique);
      } else if(max == nextAtt) {
        int newHeuristique = computeHeuristique(&state);
        if(newHeuristique > maxHeuristique) {
          max = nextAtt;
          maxHeuristique = newHeuristique;
          ret = 1;
          printf("HEURISTIQUE: %d\n",maxHeuristique);
        }
      }
    return runThrough(&state, depth+1, nextAtt, tree, ATTEMPT_TODO) || ret;
  }
  return 0;
}

void moveElements(struct state *state, int x, int y) {
  state->remainingAttempt--;
  int nx = x+1, ny = y+1;
  char tmp = state->element[x][y];
  state->element[x][y] = state->element[nx][y];
  state->element[nx][y] = state->element[nx][ny];
  state->element[nx][ny] = state->element[x][ny];
  state->element[x][ny] = tmp;
}

int destroyFallLoop(struct state *state, int attemptAhead) {
  int i, j, newAttempt = 0;
  do {
    int destroyed = 0;
    for(i=0; i<SIDE; i++) {
      for(j=0; j<SIDE; j++) {
        destroyed += destroyElem(state->element, i, j);
      }
    }
    if(destroyed == 0) {
      return attemptAhead + newAttempt;
    }
    fall(state->element);
    state->remainingAttempt = 3;
    newAttempt = 1;
  } while(1);

  return attemptAhead + newAttempt;
}

int destroyElem(char state[SIDE][SIDE], int x, int y) {

  char del = state[x][y];
  if(del == emptyToken) return 0;

  struct queue queue;
  initQueue(&queue);
  struct queue destroyed;
  initQueue(&destroyed);

  addToQueue(&queue, x, y);

  int i, res;
  int nextX[] = { -1, 0, 1, 0 };
  int nextY[] = { 0, -1, 0, 1 };

  struct nodeList *next;
  while((next = pick(&queue)) != NULL) {
    for(i=0; i<4; i++) {
      int nx = next->x+nextX[i], ny = next->y+nextY[i];
      if(nx >= 0 && nx < SIDE && ny >= 0 && ny < SIDE && state[nx][ny] == del) {
        addToQueue(&destroyed, nx, ny);
        state[nx][ny] = emptyToken;
        addToQueue(&queue, nx, ny);
      }
    }
    free(next);
  }

  res = destroyed.size;
  if(res < 4) {
    while((next = pick(&destroyed)) != NULL) {
      state[next->x][next->y] = del;
      free(next);
    }
    return 0;
  }

  while((next = pick(&destroyed)) != NULL) {
    free(next);
  }

  return res;
}

void fall(char state[SIDE][SIDE]) {
  int i, j, empty;
  for(j=0; j<SIDE; j++) {
    empty = 0;
    for(i=SIDE-1; i>=0; i--) {
      if(state[i][j] == emptyToken) {
        empty++;
      } else {
        if(empty > 0) {
          state[i+empty][j] = state[i][j];
          state[i][j] = emptyToken;
        }
      }
    }
  }
}

void printState(struct state *state, int x, int y) {
  int i, j;
  for(i=0; i<SIDE; i++) {
    for(j=0; j<SIDE; j++) {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      WORD saved_attributes;
      if((i == x || i == x+1) && (j == y || j == y+1)) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      }
      printf("%c ",state->element[i][j]);
      if((i == x || i == x+1) && (j == y || j == y+1)) {
        SetConsoleTextAttribute(hConsole, saved_attributes);
      }
    }
    printf("\n");
  }
  printf("\n");
}
