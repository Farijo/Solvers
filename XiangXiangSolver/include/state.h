#ifndef STATE
#define STATE

#include <stddef.h>

#define WIDTH 6
#define HEIGHT 7
#define NN 3

static const char emptyEmplacement[NN] = {'\0', '\0', '\0'};
static const char emptyState[WIDTH][HEIGHT][NN];
static const int scores[] = {1, 50, 300, 1000};

static const size_t emplSize = NN*sizeof(char);
static const size_t stateSize = WIDTH*HEIGHT*NN*sizeof(char);

struct state {
  char elements[WIDTH][HEIGHT][NN];
  int score;
};

void initState(struct state *state);

void setEmplacementValue(char stateEmplacement[NN], char value[NN]);

int isEmplacementOccupied(char stateEmplacement[NN]);

int computeMaximumScorePossible(struct state *state);

int compareState(struct state *state1, struct state *state2);

int isStateEmpty(struct state *state);

int areEmplacementFusionnable(struct state* state, int x1, int y1, int x2, int y2);

int toX(int emplN);

int toY(int emplN);

void executeMerge(struct state *state, int x1, int y1, int x2, int y2);

void revertMerge(struct state *state, int x1, int y1, int x2, int y2, char empl1[NN], char empl2[NN]);

int commonPoint(char empl1[NN], char empl2[NN]);

int getScore(char empl1[NN], char empl2[NN]);

int toX(int emplN);

int toY(int emplN);

void printState(struct state *state);

void printStateSimplified(struct state *state, char fill[], int x1, int y1, int x2, int y2);

#endif // STATE
