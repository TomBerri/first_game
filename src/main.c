
#include "../include/game.h"

#define BLACK RGBA8(0, 0, 0, 255)
#define LIME RGBA8(0, 255, 20, 255)
#define RED RGBA8(255, 0, 0, 255)
#define BLUE RGBA8(0, 0, 255, 255)
#define WHITE RGBA8(255, 255, 255, 255)

//Character init
int charX = 420;
int charY = SCREEN_H / 2;
int charW = 50;
int charH = 50;
int charHP = 100;

//Enemy init
int enemyX = 0;
int enemyY = 0;
int enemyW = 10;
int enemyH = 10;
int enemyMVX = -1;
int enemyMVY = -1;

//Enemy 2 init
int enemy2X = -25;
int enemy2Y = -25;
int enemy2W = 10;
int enemy2H = 10;
int enemy2MVX = -1;
int enemy2MVY = -1;

//Enemy 3 init
int enemy3X = -25;
int enemy3Y = -25;
int enemy3W = 10;
int enemy3H = 10;
int enemy3MVX = -1;
int enemy3MVY = -1;

//Timer
float timer = 0;
float timer_inc = 0.0166667;

//FPS
float FPS = 0;


int main(int argc, char *argv[]) {
	vita2d_pgf *pgf;

	vita2d_init();
	vita2d_set_clear_color(WHITE);

	pgf = vita2d_load_default_pgf();

	SceCtrlData pad;

	//Enable analog sampling
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	int circle_draw_true = 0;

	srand(time(NULL));

	while (1) {
		vita2d_start_drawing();
		vita2d_clear_screen();

		sceCtrlPeekBufferPositive(0, &pad, 1);

		vita2d_pgf_draw_text(pgf, 0, 20, BLACK, 1.0f, "Press cross to start!");

		if (pad.buttons & SCE_CTRL_CROSS) {
			vita2d_clear_screen();
			circle_draw_true = 1;
		}

		if (circle_draw_true == 1) {
			vita2d_end_drawing();
			vita2d_swap_buffers();
			sceDisplayWaitVblankStart();
			int speed = 3;

			while (circle_draw_true == 1) {
				vita2d_start_drawing();
				vita2d_clear_screen();

				sceCtrlPeekBufferPositive(0, &pad, 1);

				//Control square with d-pad
				if (pad.buttons & SCE_CTRL_UP) {
					charY -= speed;
				}
				else if (pad.buttons & SCE_CTRL_DOWN) {
					charY += speed;
				}
				else if (pad.buttons & SCE_CTRL_RIGHT) {
					charX += speed;
				}
				else if (pad.buttons & SCE_CTRL_LEFT) {
					charX -= speed;
				}

				//Control square with left analog stick
				double pi = 3.14159;
				double tx, ty;
				tx = 127.5 - pad.lx; //Center of joystick to actual joystick pos
				ty = 127.5 - pad.ly; //same as above but for y
				double angle = atan2(-ty, -tx) * 180 / pi; //Angle for movement

				//Thresholding analog stick
				if (tx < -20 || tx > 20) {
					charX += (speed*(cos(angle*pi/180))); //Move X 
				}
				if (ty < -20 || ty > 20) {
					charY += (speed*(sin(angle*pi/180))); //Move Y
				}

				//Boundary checking
				if (charX <= boundaryXLeft) {
					charX = 0;
				}
				if (charX >= boundaryXRight - charW) {
					charX = SCREEN_W - charW;
				}
				if (charY <= boundaryYUp) {
					charY = 0;
				}
				if (charY >= boundaryYDown- charH) {
					charY = SCREEN_H - charH;
				}

				//Exit with start
				if (pad.buttons & SCE_CTRL_START) {
					circle_draw_true = 0;
					break;
				}

				//Speed up and down with triggers
				if (pad.buttons & SCE_CTRL_RTRIGGER) {
					speed++;
				}
				if ((pad.buttons & SCE_CTRL_LTRIGGER) && (speed > 0)) {
					speed--;
				}

				//Program enemy
				if (enemyAI(&enemyX, &enemyY, &enemyMVX, &enemyMVY) == -1) {
					vita2d_end_drawing();
					vita2d_swap_buffers();
					vita2d_fini();
					vita2d_free_pgf(pgf);
					sceKernelExitProcess(-1);
					return -1;
				}

				//Move enemy by vector
				enemyX += enemyMVX;
				enemyY += enemyMVY;

				//Program enemy 2
				if (timer > 30) {
					if (enemyAI(&enemy2X, &enemy2Y, &enemy2MVX, &enemy2MVY) == -1) {
						vita2d_end_drawing();
						vita2d_swap_buffers();
						vita2d_fini();
						vita2d_free_pgf(pgf);
						sceKernelExitProcess(-1);
						return -1;
					}

					//Move enemy2 by vector
					enemy2X += enemy2MVX;
					enemy2Y += enemy2MVY;
				}
					
				//Program enemy 3
				if (timer > 60) {
					if (enemyAI(&enemy3X, &enemy3Y, &enemy3MVX, &enemy3MVY) == -1) {
						vita2d_end_drawing();
						vita2d_swap_buffers();
						vita2d_fini();
						vita2d_free_texture(test);
						vita2d_free_pgf(pgf);
						sceKernelExitProcess(-1);
						return -1;
					}

					//Move enemy by vector
					enemy3X += enemy3MVX;
					enemy3Y += enemy3MVY;
				}

				//Collision detection
				int diffX = abs(charX - enemyX);
				int diffY = abs(charY - enemyY);
				int diff2X = abs(charX - enemy2X);
				int diff2Y = abs(charY - enemy2Y);
				int diff3X = abs(charX - enemy3X);
				int diff3Y = abs(charY - enemy3Y);

				//Enemy collision
				if ((diffX < charW) && (diffY < charH)) {
					vita2d_pgf_draw_text(pgf, 500, 100, RED, 1.0f, "COLLISION!");
					charHP--;
					//Game Over check
					if (charHP <= 0) {
						circle_draw_true = 0;
						break;
					}
				}

				//Enemy 2 collision 
				if ((diff2X < charW) && (diff2Y < charH)) {
					vita2d_pgf_draw_text(pgf, 500, 100, RED, 1.0f, "COLLISION!");
					charHP--;
					//Game Over check
					if (charHP <= 0) {
						circle_draw_true = 0;
						break;
					}
				}

				//Enemy 3 collision
				if ((diff3X < charW) && (diff3Y < charH)) {
					vita2d_pgf_draw_text(pgf, 500, 100, RED, 1.0f, "COLLISION!");
					charHP--;
					//Game Over check
					if (charHP <= 0) {
						circle_draw_true = 0;
						break;
					}
				}

				//Increment timer
				timer += timer_inc;

				//FPS counter
				sceDisplayGetRefreshRate(&FPS);

				//malloc strings for HP, timer, FPS
				char *cHP_str = (char *) malloc(sizeof (char) * 8);
				char *timer_str = (char *) malloc(sizeof(char) * 70);
				char *FPS_str = (char *) malloc(sizeof(char) * 10);

				//Set strings for HP, timer, FPS
				sprintf(cHP_str, "HP: %d", charHP);
				sprintf(timer_str, "Time: %.3f", timer);
				sprintf(FPS_str, "FPS: %.0f", FPS);

				//Draw the character
				vita2d_draw_rectangle(charX, charY, charW, charH, BLUE);

				//Draw enemies
				if (timer < 100) {
					vita2d_draw_rectangle(enemyX, enemyY, enemyW, enemyH, RED);
					vita2d_draw_rectangle(enemy2X, enemy2Y, enemy2W, enemy2H, RED);
					vita2d_draw_rectangle(enemy3X, enemy3Y, enemy3W, enemy3H, RED);
				}
				else {
					vita2d_draw_rectangle(enemyX, enemyY, enemyW * 3, enemyH * 3, RED);
					vita2d_draw_rectangle(enemy2X, enemy2Y, enemy2W * 3, enemy2H * 3, RED);
					vita2d_draw_rectangle(enemy3X, enemy3Y, enemy3W * 3, enemy3H * 3, RED);
				}

				//Character HP and HP bar
				vita2d_pgf_draw_text(pgf, 0, SCREEN_H - 40, BLACK, 1.0f, cHP_str);
				vita2d_draw_rectangle(0, SCREEN_H - 30, charHP, 30, LIME);

				//Timer
				vita2d_pgf_draw_text(pgf, 0, 20, BLACK, 1.0f, timer_str);

				//FPS
				vita2d_pgf_draw_text(pgf, SCREEN_W - 85, 20, BLACK, 1.0f, FPS_str);

				//End circle_draw_true drawing and swap buffers
				vita2d_end_drawing();
				vita2d_swap_buffers();				
			}
		}
		//Reset game
		charX = 420;
		charY = SCREEN_H / 2;
		enemy2X = -25;
		enemy2Y = -25;
		enemy3X = -25;
		enemy3Y = -25;
		charHP = 100;
		timer = 0;

		//End initial drawing and swap buffers
		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceDisplayWaitVblankStart();
	}

	//End program
	vita2d_fini();
	vita2d_free_texture(test);
	vita2d_free_pgf(pgf);
	sceKernelExitProcess(0);
	return 0;
}