#include "../include/game.h"

/*
* Randomises the speed and movement of an enemy if it has gone off-screen
*/
int enemyAI(int *X, int *Y, int *MVX, int *MVY) {
	if ((*X < boundaryXLeft) || (*X > boundaryXRight) || (*Y <= boundaryYUp) || (*Y >= boundaryYDown)) {
		//Randomise starting position
		int rand_pos = rand() % 4;
			switch(rand_pos) {
				case(0):
					*X = 0;
					*MVX = (rand() % 10) + 5;
					break;
				case(1):
					*X = SCREEN_W;
					*MVX = -((rand() % 10) + 5);
					break;
				case(2):
					*Y = 0;
					*MVY = (rand() % 10) + 5;
					break;
				case(3):
					*Y = SCREEN_H;
					*MVY = -((rand() % 10) + 5);
					break;	
				default:
					//Error case
					return -1;
			}

			if (rand_pos == 0 || rand_pos == 1) {
				*Y = rand() % SCREEN_H + 1;
				do {
					*MVY = (rand() % 20) - 10;
				} while (*MVY < 5 && *MVY > -5);
			}
			else {
				*X = rand() % SCREEN_W + 1;
				do {
					*MVX = (rand() % 20) - 10;
				} while (*MVY < 5 && *MVY > -5);
			} 
	}

	return 0;
}