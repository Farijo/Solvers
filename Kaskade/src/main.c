#include <soluceNode.h>
#include <state.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void analyseState(char state[SIDE][SIDE]) {

}

int main(int argc, char **argv) {
  struct state state;
  initState(&state);
  char e[SIDE][SIDE] = {
    "vvvbvrvb",
    "vvbvvvbr",
    "bbvrbbrb",
    "brrvbrrv",
    "vvvrrbvr",
    "rrbbvrbv",
    "vvvvvrbb",
    "vbbbbvvv"
  };
  memcpy(state.element, e, SIDE*SIDE);
  struct done done;
  initdone(&done);
  runThrough(&state, done, 0);
  return 0;
}
