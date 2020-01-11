#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

// DO NOT remove following variables
int time_out, game_over;

/* Usage of other user function is also allowed */


void drawSnake(){
    /* TODO: draw whole snake */
    
 }


void makeItem() {
	/* 
     * TODO: generate COOR for each item and place them on map[][] 
     * 1. Feed: add node at the end of the snake
     * 2. Poison: delete node at the end of the snake
     * 3. Snack: change pattern of first body node
     */

}

void drawBoard() {
    
    system("clear"); // clear screen
    puts("");
    printf("Snake Game\n");
    puts("");
 
    /* TODO: draw Board, snake and items*/

}

void init() {
	/*
     * TODO:
     * 1. read the map from board.txt
     * 2. malloc SNAKE node
     * 3. malloc SNAKE_NODE face node
     * 4. make initial position of items
     */
    // user code
 

    // user code end
	game_over = 0;
	time_out = 0;
}
   




/* call by ualarm( microsec ) */

int temp_do_count = 0;

void Do(int sig) {
	system("clear");
	//printf("Alarm %d!\n", ++temp_do_count);


    /* TODO:
     * 1. set the COOR of the snake by its heading direction
     * 2. what to do when face meets an item(F, P, S)
     * 3. game_over condition 
     */


    // 3. game_over condition 

    /* time out variable update */
	time_out = 0;
}

/* get key charater directly */
int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0; /* RIGHT */
	case 'w':
	case 'W': return 1; /* UP */
	case 'a':
	case 'A': return 2; /* LEFT */
	case 's':
	case 'S': return 3; /* DOWN */
	default: return -1; /* OTHER KEY */
	}

	return -1;
}


int main() {
	int command;
	init();
	srand(time(NULL));
	static struct sigaction act, oact;

	/* if call alarm or ualarm function, then call Do function/ */
	act.sa_handler = Do; /* ( signal.h header ) */
	sigaction(SIGALRM, &act, NULL);

	do {

		if (time_out == 0) {
			// alarm(1); // 1 second
			ualarm(300000, 0); // 0.3 second ( unistd.h header )
            time_out = 1;
		}

        command = GetCommand();

        // user code

        //drawBoard();

	} while (!game_over);

	//system("clear");
	printf("game over!\n");
}
