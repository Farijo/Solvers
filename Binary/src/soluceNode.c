#include <soluceNode.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int addStateToTree(struct soluceNode *node, struct state *state) {
  int diff = compareState(&node->state, state);
  if(diff == 0) {
    return 0;
  }
  if(diff > 0) {
    if(node->left) {
      return addStateToTree(node->left, state);
    } else {
      node->left = createNode(state);
      return 1;
    }
  } else {
    if(node->right) {
      return addStateToTree(node->right, state);
    } else {
      node->right = createNode(state);
      return 1;
    }
  }
}

int treeSize(struct soluceNode * tree) {
  int res = 1;
  if(tree->left) {
    res += treeSize(tree->left);
  }
  if(tree->right) {
    res += treeSize(tree->right);
  }
  return res;
}

int runThrough(struct state *state, int depth, int attemptAhead, struct soluceNode *tree, int ATTEMPT_TODO[3][2]) {
  if(!addStateToTree(tree, state)) {
    return 0;
  }
  int i, j, LAST = SIDE-1, ret = 0;
  for(i=0; i<LAST; i++) {
    for(j=0; j<LAST; j++) {
      if(isAttemptValid(state, i, j)) {
        if(executeAttempt(*state, i, j, depth, attemptAhead, tree, ATTEMPT_TODO)) {
          char fill[depth+1];
          memset(fill, ' ', depth);
          fill[depth] = 0;
          printf("%s%d %d\n",fill, i, j);
          if(attemptAhead == 1) {
            printStateToFile(state);

            ATTEMPT_TODO[0][0] = -1;
            ATTEMPT_TODO[0][1] = -1;
            ATTEMPT_TODO[1][0] = -1;
            ATTEMPT_TODO[1][1] = -1;
            ATTEMPT_TODO[2][0] = -1;
            ATTEMPT_TODO[2][1] = -1;
          }
          ret = 1;
          printState(state, i, j);
          if(depth < 3 && attemptAhead == 0) {
            ATTEMPT_TODO[depth][0] = i;
            ATTEMPT_TODO[depth][1] = j;
          }
        }
      }
    }
  }
  return ret;
}

void printStateToFile(struct state *state) {
  FILE *fp = fopen("state", "w");
  int k, l;
  for(k=0; k<SIDE; k++) {
    for(l=0; l<SIDE; l++) {
      fputc(state->element[k][l], fp);
    }
    fputc('\n',fp);
  }
  fclose(fp);
}
