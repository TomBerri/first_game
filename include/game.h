#ifndef game_h
#define game_h

#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SCREEN_W 960
#define SCREEN_H 544

//Boundaries
#define boundaryXLeft 0
#define boundaryXRight SCREEN_W
#define boundaryYUp 0
#define boundaryYDown SCREEN_H

int enemyAI(int *X, int *Y, int *MVX, int *MVY);

#endif