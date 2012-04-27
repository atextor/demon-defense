#include "main.h"

#define IMPS 8 

int main(int argc, char *argv[]) {
	bool running;
	int i;
	Uint32 textColor;
	SDL_Surface *bg = SDL_LoadBMP("level.bmp");

	Unit *imp[IMPS];
	Unit *player;

	srand(time(NULL));
	if (sgInit(false) != 0) {
		exit(EXIT_FAILURE);
	}
	wadInit(WADFILE);
	unitInit();
	for (i = 0; i < IMPS; i++) {
		imp[i] = newUnit(rand() % 4 + 1, 75, (i * 60));
		/*imp[i] = newUnit(BARONOFHELL, 75, (i * 60));*/
	}
	player = newUnit(PLAYER, 100, 150);

	textColor = sgCreateColor(0, 255, 255);
	running = true;
	while (running) {
		loopDelay();
		sgDrawImage(bg, 0, 0);
		/* Funky characters are only there to test the UTF-8 text output */
		sgDrawText("░░▒▒▓▓██ ÐǼṀǾŇ ΔΞFΣΠƧƎ ▓▓▒▒░░", 50, 20, textColor);
		for (i = 0; i < IMPS; i++) {
			unitMove(imp[i]);
			unitDraw(imp[i]);
		}
		unitFindTarget(player, imp[0]);
		unitDraw(player);
		sgUpdateScreen();
		running = checkInput();
	}

	SDL_Quit();
	return 0;
}

