#include <state.h>
#include <soluceNode.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

  memset(emptyState, '\0', WIDTH*HEIGHT*NN*sizeof(char));

  struct soluceNode node;
  initNode(&node);
  node.state.score = -1;
  struct state s;
  initState(&s);
  char f[WIDTH][HEIGHT][NN] = {
    { "tbv", "crx", "cvt", "cjx", "rvp", "rvp", "rvc" },
    { "cvt", "tjp", "tjp", "tvx", "cjx", "rvp", "cbv" },
    { "rjt", "cvp", "tjc", "cjp", "crv", "cjt", "rvp" },
    { "tvv", "tbc", "rjt", "tbp", "rvp", "cvc", "rvt" },
    { "cvt", "cjc", "rjx", "trp", "trt", "rbc", "tbc" },
    { "crc", "rvc", "tvp", "tvt", "rvc", "cjv", "tjx" }
  };
  if(argc == 43) {
    int i;
    for(i=1; i<argc; i++) {
      strncpy(f[toX(i-1)][toY(i-1)], argv[i], NN);
    }
  }
  memcpy(s.elements, f, sizeof(f));
  printf("MAX : %d\n",computeMaximumScorePossible(&s));
  runThroughState(&s, &node, 0);
  freeTree(&node);
  return 0;
}
