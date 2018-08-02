#include <soluceNode.h>
#include <state.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#define FX 2222
#define FY 433

static const int mousePos[SIDE][SIDE][2] = {
  {{FX, FY},
  {FX+40, FY},
  {FX+80, FY},
  {FX+120, FY}},
  {{FX, FY+40},
  {FX+40, FY+40},
  {FX+80, FY+40},
  {FX+120, FY+40}},
  {{FX, FY+80},
  {FX+40, FY+80},
  {FX+80, FY+80},
  {FX+120, FY+80}},
  {{FX, FY+120},
  {FX+40, FY+120},
  {FX+80, FY+120},
  {FX+120, FY+120}}
};

void clicL(int x, int y) {
  Sleep(600);
  SetCursorPos(x-5, y);
  Sleep(90);
  SetCursorPos(x, y-5);
  Sleep(90);
  SetCursorPos(x+5, y);
  Sleep(90);
  SetCursorPos(x, y+5);
  Sleep(90);
  SetCursorPos(x, y);
  Sleep(100);
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
  Sleep(80);
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void clic(const int pos[2]) {
  clicL(pos[0], pos[1]);
}

int main(int argc, char **argv) {
  struct state state;
  initState(&state);
  struct soluceNode tree;
  initNode(&tree);
  char e[SIDE][SIDE] = {
    "-----",
    "-----",
    "-----",
    "-----",
    "-----"
  };
  memcpy(state.element, e, SIDE*SIDE);

  FILE *fp = fopen("state", "r");
  char ch;
  int i = 0, j = 0, fill = 0, FILL_LEN = argc > 1 ? strlen(argv[1]) : 0;
  while((ch = fgetc(fp)) != EOF) {
    if('a' <= ch && ch <= 'z') {
      state.element[i][j] = ch;printf("%c ",ch);
    } else if (ch == emptyToken) {
      if(fill < FILL_LEN) {
        state.element[i][j] = argv[1][fill++];
      } else {
        printf("Il manque des arguments !\n");
        return 0;
      }
    } else {
      continue;
    }
    j++;
    if(j == SIDE) {
      i++;
      j = 0;
    }
  }
  fclose(fp);

  int ATTEMPT_TODO[3][2] = { {-1, -1}, {-1, -1}, {-1, -1} };
  runThrough(&state, 0, 0, &tree, ATTEMPT_TODO);

  for(i=0; i<3; i++) {
    int x, y;
    x = ATTEMPT_TODO[i][0];
    y = ATTEMPT_TODO[i][1];
    if(x != -1 && y != -1) {
      clic(mousePos[x][y]);
    }
  }
  clicL(1300, 630);

  return 0;
}
