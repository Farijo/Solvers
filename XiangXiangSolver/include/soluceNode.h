#ifndef SOLUCENODE
#define SOLUCENODE

#include <state.h>

#define M 50

struct soluceNode {
  struct state state;
  struct soluceNode* left;
  struct soluceNode* right;
};

void initNode(struct soluceNode *node);

struct soluceNode* createNode(struct state* state);

/// return 1 if the state did not exists or
/// if the new state score is higher than
/// the previous score of the node state
/// return 0 otherwise
int addOrUpgradeStateToTree(struct soluceNode *node, struct state *state);

void freeTree(struct soluceNode *root);

int runThroughState(struct state *state, struct soluceNode *tree, int depth);

int _print_t(struct soluceNode *tree, int is_left, int offset, int depth, char s[M][255]);

void print_t(struct soluceNode *tree);

#endif // SOLUCENODE

