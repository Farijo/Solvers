#include <state.h>

#include <stdio.h>
#include <string.h>
#include <windows.h>

void initState(struct state *state) {
  memset(state->elements, '\0', stateSize);
  state->score = 0;
}

void removeEmplacementValue(char stateEmplacement[NN]) {
  memset(stateEmplacement, '\0', emplSize);
}

void setEmplacementValue(char stateEmplacement[NN], char value[NN]) {
  memcpy(stateEmplacement, value, emplSize);
}

int isEmplacementOccupied(char stateEmplacement[NN]) {
  return memcmp(stateEmplacement, emptyEmplacement, emplSize);
}

int corres(char v) {
  switch(v) {
    case 'c':
    case 'b':
      return 0; // carré bleu carré
    case 'r':
    case 'p':
      return 1; // rond rouge point
    case 't':
    case 'j':
      return 2; // triangle jaune triangle
    case 'v':
      return 3; // vert vague
    case 'x':
      return 4; // croix
  }
  printf("ERROR unkown symbol\n");
  return -1;
}

static const int FORM = 3, COLOR = 4, SYMBOL = 5;

int consumeElemForScore(int fcs[FORM][COLOR][SYMBOL], int iter[3],
                        int *save1, int curInd1,
                        int *save2, int curInd2,
                        int score) {
  // si on a trouvé un élément
  if(fcs[iter[0]][iter[1]][iter[2]]) {
    // si on a déjà trouvé son pair
    if(*save1 >= 0 && *save2 >= 0) {
      // on reset les cases correspondantes de fcs et on retourne le score
      fcs[iter[0]][iter[1]][iter[2]] = 0;
      iter[curInd1] = *save1;
      iter[curInd2] = *save2;
      fcs[iter[0]][iter[1]][iter[2]] = 0;
      *save1 = -1;
      *save2 = -1;
      return score;
    } else {
      // on sauvegarde l'élément trouvé
      *save1 = iter[curInd1];
      *save2 = iter[curInd2];
    }
  }
  return 0;
}

/// calcul du score sans tenir compte de l'accessibilité
int computeMaximumScorePossible(struct state *state) {
  int i, j, k, last1, last2, fcs[FORM][COLOR][SYMBOL], potential = 0;
  memset(fcs, 0, sizeof(fcs));

  // on transforme state en tableau 3D (fcs) où chaque dimension
  // correspond à un point commun [FORM][COLOR][SYMBOL]
  // dans le même temps on consomme les couples parfait (1000 pts)
  for(i=0; i<WIDTH; i++) {
    for(j=0; j<HEIGHT; j++) {
      if(!isEmplacementOccupied(state->elements[i][j])) {
        continue;
      }
      int f=corres(state->elements[i][j][0]), c=corres(state->elements[i][j][1]), s=corres(state->elements[i][j][2]);
      if(fcs[f][c][s]) {
        potential += scores[3];
        fcs[f][c][s] = 0;
      } else {
        fcs[f][c][s] = 1;
      }
    }
  }

  int iter[3], dimLen[] = {FORM, COLOR, SYMBOL}, indexes[] = {1,2,0,1};

  // on parcours toutes les colonnes de fcs, c.a.d.
  // i, j, k coordonnées de dimensions de fcs
  // tel que i==j ou j==k ou i==k (itérateurs
  // des deux premières boucles) et respectivement k, i, j
  // l'iterateur de la boucle la plus profonde
  for(i=0; i<3; i++) {
    int index1D = i;
    for(iter[index1D]=0; iter[index1D]<dimLen[index1D]; iter[index1D]++) {
      int index2D = indexes[i];
      for(iter[index2D]=0; iter[index2D]<dimLen[index2D]; iter[index2D]++) {
        int index3D = indexes[i+1];
        last1 = -1;
        for(iter[index3D]=0; iter[index3D]<dimLen[index3D]; iter[index3D]++) {
          potential += consumeElemForScore(fcs, iter, &last1, index3D, &last1, index3D, scores[2]);
        }
      }
    }
  }

  // parcours de chaque plan
  for(i=0; i<3; i++) {
    int index1D = i;
    for(iter[index1D]=0; iter[index1D]<dimLen[index1D]; iter[index1D]++) {
      int index2D = indexes[i];
      last1 = last2 = -1;
      for(iter[index2D]=0; iter[index2D]<dimLen[index2D]; iter[index2D]++) {
        int index3D = indexes[i+1];
        for(iter[index3D]=0; iter[index3D]<dimLen[index3D]; iter[index3D]++) {
          potential += consumeElemForScore(fcs, iter, &last1, index3D, &last2, index2D, scores[1]);
        }
      }
    }
  }

  // on consomme les éléments solitaires
  int elem = 0;
  for(i=0; i<FORM; i++) {
    for(j=0; j<COLOR; j++) {
      for(k=0; k<SYMBOL; k++) {
        elem += fcs[i][j][k];
      }
    }
  }
  potential += (elem / 2) * scores[0];

  return state->score + potential;
}

int compareState(struct state *state1, struct state *state2) {
  return memcmp(state1->elements, state2->elements, stateSize);
}

int isStateEmpty(struct state *state) {
  return !memcmp(state->elements, emptyState, stateSize);
}

int areEmplacementFusionnable(struct state* state, int x1, int y1, int x2, int y2) {

  if(!isEmplacementOccupied(state->elements[x1][y1])) {
    return 0;
  }
  if(!isEmplacementOccupied(state->elements[x2][y2])) {
    return 0;
  }

  int i, path1 = 1, path2 = 1;
  if(x1 > x2) {
    i = x1;
    x1 = x2;
    x2 = i;
  }
  for(i=x1+1; i<x2; i++) {
    if(isEmplacementOccupied(state->elements[i][y1])) {
      path1 = 0;
      break;
    }
  }
  for(i=x1; i<x2; i++) {
    if(isEmplacementOccupied(state->elements[i][y2])) {
      if(!path1) {
        return 0;
      }
      path2 = 0;
      break;
    }
  }

  if(y1 > y2) {
    i = y1;
    y1 = y2;
    y2 = i;
  }
  for(i=y1+1; i<y2; i++) {
    if(isEmplacementOccupied(state->elements[x1][i])) {
      if(!path1) {
        return 0;
      }
      path2 = 0;
      break;
    }
  }
  for(i=y1; i<y2; i++) {
    if(isEmplacementOccupied(state->elements[x2][i])) {
      if(!path2) {
        return 0;
      }
      path1 = 0;
      break;
    }
  }

  return 1;
}

int toX(int emplN) {
  return emplN / HEIGHT;
}

int toY(int emplN) {
  return emplN % HEIGHT;
}

void executeMerge(struct state* state, int x1, int y1, int x2, int y2) {
  state->score += getScore(state->elements[x1][y1], state->elements[x2][y2]);
  removeEmplacementValue(state->elements[x1][y1]);
  removeEmplacementValue(state->elements[x2][y2]);
}

void revertMerge(struct state *state, int x1, int y1, int x2, int y2, char empl1[NN], char empl2[NN]) {
  int score = getScore(empl1, empl2);
  setEmplacementValue(state->elements[x1][y1], empl1);
  setEmplacementValue(state->elements[x2][y2], empl2);
  state->score -= score;
}

int commonPoint(char empl1[NN], char empl2[NN]) {
  int i, common = 0;
  for(i=0; i<NN; i++) {
    if(empl1[i] == empl2[i]) {
      common++;
    }
  }
  return common;
}

int getScore(char empl1[NN], char empl2[NN]) {
  return scores[commonPoint(empl1, empl2)];
}

void printState(struct state *state) {
  printf("\n%d\n",state->score);
  int i, j, k;
  for(i=0; i<HEIGHT; i++) {
    for(j=0; j<WIDTH; j++) {
      for(k=0; k<NN; k++) {
        printf("%d", state->elements[j][i][k]);
      }
      printf(" ");
    }
    printf("\n");
  }
}

void printStateSimplified(struct state *state, char fill[], int x1, int y1, int x2, int y2) {
  printf("%s%d\n",fill, state->score);
  int i, j;
  for(i=0; i<HEIGHT; i++) {
    printf("%s", fill);
    for(j=0; j<WIDTH; j++) {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      WORD saved_attributes;
      if((j==x1&&i==y1) || (j==x2&&i==y2)) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      }
      printf("%c ", isEmplacementOccupied(state->elements[j][i]) ? '-' : '0');
      if((j==x1&&i==y1) || (j==x2&&i==y2)) {
        SetConsoleTextAttribute(hConsole, saved_attributes);
      }
    }
    printf("\n");
  }
  printf("\n");
}
