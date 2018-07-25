#include <soluceNode.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void initNode(struct soluceNode *node) {
  initState(&node->state);
  node->left = 0;
  node->right = 0;
}

struct soluceNode* createNode(struct state* state) {
  struct soluceNode *newNode = (struct soluceNode*)malloc(sizeof(struct soluceNode));
  initNode(newNode);
  newNode->state = *state;
  return newNode;
}

int addOrUpgradeStateToTree(struct soluceNode *node, struct state *state) {
  int diff = compareState(&node->state, state);
  if(diff == 0) {
    if(state->score > node->state.score) {
      node->state.score = state->score;
      return 1;
    }
    return 0;
  }
  if(diff > 0) {
    if(node->left) {
      return addOrUpgradeStateToTree(node->left, state);
    } else {
      node->left = createNode(state);
      return 1;
    }
  } else {
    if(node->right) {
      return addOrUpgradeStateToTree(node->right, state);
    } else {
      node->right = createNode(state);
      return 1;
    }
  }
}

void freeTree(struct soluceNode *root) {
  if(root->left) {
    freeTree(root->left);
  }
  if(root->right) {
    freeTree(root->right);
  }
  free(root);
}

int getNegligeableScoreDiff(int score) {
  if(score < 10000) {
    return 10000 - score;
  }
  if(score < 12000) {
    return 1000;
  }
  if(score < 13000) {
    return 0;
  }
  return 0;
}

int runThroughState(struct state *state, struct soluceNode *tree, int depth) {
  if(!addOrUpgradeStateToTree(tree, state)) {
    return 0;
  }
  if(isStateEmpty(state)) {
    printf("SCORE=%d\n",state->score);
    return 1;
  }
  if(((WIDTH*HEIGHT/2 - depth)*1000 + state->score) < tree->state.score) {
    return 0;
  }
  if(computeMaximumScorePossible(state)-getNegligeableScoreDiff(tree->state.score) < tree->state.score) {
    return 0;
  }

  int i, j, totalEmplacementNum = WIDTH*HEIGHT, res = 0;
  int possible[4][(totalEmplacementNum*totalEmplacementNum+1)*2];
  int posIter[4] = {0, 0, 0, 0};
  for(i=0; i<totalEmplacementNum; i++) {
    int x1 = toX(i), y1 = toY(i);
    if(!isEmplacementOccupied(state->elements[x1][y1])) {
      continue;
    }
    for(j=i+1; j<totalEmplacementNum; j++) {
      int x2 = toX(j), y2 = toY(j);
      if(!isEmplacementOccupied(state->elements[x2][y2])) {
        continue;
      }
      if(areEmplacementFusionnable(state, x1, y1, x2, y2)) {
          int p = commonPoint(state->elements[x1][y1], state->elements[x2][y2]);
          possible[p][posIter[p]++] = x1;
          possible[p][posIter[p]++] = y1;
          possible[p][posIter[p]++] = x2;
          possible[p][posIter[p]++] = y2;
      }
    }
  }

  char empl1[NN], empl2[NN];
  for(i=3; i>=0; i--) {
    for(j=0; j<posIter[i]; j+=4) {
      int x1 = possible[i][j], y1 = possible[i][j+1], x2 = possible[i][j+2], y2 = possible[i][j+3];
      setEmplacementValue(empl1, state->elements[x1][y1]);
      setEmplacementValue(empl2, state->elements[x2][y2]);
      executeMerge(state, x1, y1, x2, y2);
      if(runThroughState(state, tree, depth+1)) {
        char fill[depth+1];
        memset(fill, ' ', sizeof(fill));
        fill[depth] = '\0';
        printf("%saction [%d,%d][%d,%d]\n", fill, x1, y1, x2, y2);
        printStateSimplified(state, fill, x1, y1, x2, y2);

        res = 1;
      }
      revertMerge(state, x1, y1, x2, y2, empl1, empl2);
      if(i == 3) {
        return res;
      }
    }
  }

  return res;
}

int _print_t(struct soluceNode *tree, int is_left, int offset, int depth, char s[M][255]) {
  char b[M];
  int width = 5, i;

  if (!tree) return 0;

  sprintf(b, "(%03d)", tree->state.score);

  int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
  int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

  for (i = 0; i < width; i++) {
    s[2 * depth][offset + left + i] = b[i];
  }

  if (depth && is_left) {
    for (i = 0; i < width + right; i++) {
      s[2 * depth - 1][offset + left + width/2 + i] = '-';
    }
    s[2 * depth - 1][offset + left + width/2] = '+';
    s[2 * depth - 1][offset + left + width + right + width/2] = '+';
  } else if (depth && !is_left) {
    for (i = 0; i < left + width; i++) {
      s[2 * depth - 1][offset - width/2 + i] = '-';
    }
    s[2 * depth - 1][offset + left + width/2] = '+';
    s[2 * depth - 1][offset - width/2 - 1] = '+';
  }
  return left + width + right;
}

void print_t(struct soluceNode *tree) {
  char s[M][255];
  int i;
  for (i = 0; i < M; i++) {
    sprintf(s[i], "%80s", " ");
  }

  _print_t(tree, 0, 0, 0, s);

  for (i = 0; i < M; i++) {
    printf("%s\n", s[i]);
  }
}
