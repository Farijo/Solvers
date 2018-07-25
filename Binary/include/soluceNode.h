#ifndef SOLUCENODE
#define SOLUCENODE

#include <state.h>

struct soluceNode {
  struct state state;
  struct soluceNode* left;
  struct soluceNode* right;
};

void initNode(struct soluceNode *node);

struct soluceNode* createNode(struct state* state);

int addStateToTree(struct soluceNode *node, struct state *state);

int runThrough(struct state *state, int depth, int attemptAhead, struct soluceNode *tree, int ATTEMPT_TODO[3][2]);

void printStateToFile(struct state *state);

#endif // SOLUCENODE
