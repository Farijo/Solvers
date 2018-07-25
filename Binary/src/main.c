#include <soluceNode.h>
#include <state.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#define FX 155
#define FY 440

//#define WHERE_CURSOR 1

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

void getColors() {
  HDC hdc, hdcTemp;
  RECT rect;
  BYTE* bitPointer;
  int x, y, i;
  int red, green, blue, alpha;

  hdc = GetDC(HWND_DESKTOP);
  GetWindowRect(HWND_DESKTOP, &rect);
  int MAX_WIDTH = rect.right;
  int MAX_HEIGHT = rect.bottom;

  hdcTemp = CreateCompatibleDC(hdc);

  BITMAPINFO bitmap;
  bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
  bitmap.bmiHeader.biWidth = MAX_WIDTH;
  bitmap.bmiHeader.biHeight = MAX_HEIGHT;
  bitmap.bmiHeader.biPlanes = 1;
  bitmap.bmiHeader.biBitCount = 32;
  bitmap.bmiHeader.biCompression = BI_RGB;
  bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
  bitmap.bmiHeader.biClrUsed = 0;
  bitmap.bmiHeader.biClrImportant = 0;
  HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
  SelectObject(hdcTemp, hBitmap2);
  BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, 0, 0, SRCCOPY);

  for (i=0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i+=4) {
    red = (int)bitPointer[i];
    green = (int)bitPointer[i+1];
    blue = (int)bitPointer[i+2];
    alpha = (int)bitPointer[i+3];
    if(red == 0)return;
    printf("%d %d %d\n",red, green, blue);
  }
}

int main(int argc, char **argv) {
  struct state state;
  initState(&state);
  struct soluceNode tree;
  initNode(&tree);
  char e[SIDE][SIDE] = {
    "jmzmr",
    "rivvj",
    "ovcrm",
    "ivcoi",
    "cbivi"
  };
  memcpy(state.element, e, SIDE*SIDE);

  #ifdef WHERE_CURSOR

  POINT p;
  while(1) {
    GetCursorPos(&p);
    printf("%ld %ld\n",p.x, p.y);
  }

  #endif

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
    if(x != -1 && y != -1)
      clic(mousePos[x][y]);
  }
  clicL(1050, 400);

  return 0;
}
