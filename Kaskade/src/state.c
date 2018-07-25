#include <state.h>
#include <stdio.h>
#include <windows.h>

void initState(struct state *state) {
  memset(state->element, '-', SIDE*SIDE);
  state->score = 0;
  state->destroyed = 0;
}

void initdone(struct done *done){
  memset(done->element, 0, SIDE*SIDE);
}

int getScoreElementDestroyed(int destroyed) {
  return destroyed * (destroyed - 1) * 50;
}

void printState(struct state *state, struct done *before, struct done *done) {
  int i, j;
  printf("%d (%d)\n",state->score, state->destroyed);
  for(i=0; i<SIDE; i++) {
    for(j=0; j<SIDE; j++) {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      WORD saved_attributes;
      if(!before->element[i][j] && done->element[i][j]) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      }
      printf("%c ",state->element[i][j]);
      if(!before->element[i][j] && done->element[i][j]) {
        SetConsoleTextAttribute(hConsole, saved_attributes);
      }
    }
    printf("\n");
  }
  printf("\n");
}
